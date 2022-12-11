#include "SQLdataBase.h"

namespace fs = std::filesystem;
using namespace std;

void SQLdataBase::setLogger(shared_ptr<Logger> logger) {
	logger_ = logger;
}

bool SQLdataBase::isConnectedToSQLdataBase() {
	mysql_init(&mysql_);
	if (&mysql_ == NULL) {
		*logger_ << "Error: can't create MySQL-descriptor" << endl;
		return false;
	}
	// Подключаемся к серверу
	if (!mysql_real_connect(&mysql_, "localhost", startData.SQL_user,
	                        startData.SQL_password, startData.SQL_name, 0, NULL, 0)) {
		*logger_ << "Error: can't connect to database " << endl;
		return false;
	} else {
		*logger_ << "Connected to SQL database" << endl;
	}

	mysql_set_character_set(&mysql_, "utf8");

	createTables();
	addUsersToSQLFromFile();
	return true;
}

void SQLdataBase::addUsersToSQLFromFile() {
	*logger_ << "Inserting users to SQL from file" << endl;
	fstream user_file_stream = fstream(startData.users_file,
	                                   ios::in | ios::out);

	string login, password;
	AuthData authdata;
	uint* password_sha1_hash;
	string command;
	vector<string> results;

	if (!user_file_stream) {
		user_file_stream = fstream(startData.users_file,
		                           ios::in | ios::out | ios::trunc);
		fs::permissions(startData.users_file,fs::perms::all,
		                fs::perm_options::remove);
		fs::permissions(startData.users_file,
		                fs::perms::owner_read | fs::perms::owner_write,
		                fs::perm_options::replace);
		*logger_ << "Created file <" << startData.users_file
		         << "> with permissions: "
		         << showPermissions(fs::status(startData.users_file).permissions())
		         << endl;
	} else {
		fs::permissions(startData.users_file, fs::perms::all,
		                fs::perm_options::remove);
		fs::permissions(startData.users_file,
		                fs::perms::owner_read | fs::perms::owner_write,
		                fs::perm_options::replace);
//		logger_->log(string("Opened file <" + startData.users_file +
//		                    "> with permissions: " + showPermissions(fs::status(
//		                                startData.users_file).permissions())));
		*logger_ << "Opened file <" << startData.users_file
		         << "> with permissions: "
		         << showPermissions(fs::status(startData.users_file).permissions())
		         << endl;


		while (!user_file_stream.eof()) {

			user_file_stream >> login >> password;

			command = "INSERT INTO users (login) VALUES ('" + login + "')";
			mysql_query(&mysql_, command.c_str());
			*logger_ << command << endl;

			command = "SELECT id FROM users WHERE login = '" + login + "'";
			mysql_query(&mysql_, command.c_str());
			*logger_ << command << endl;

			results = getResultsFromSQL();
			if (results.size()!=0) {
				//cout << results[0] << endl;

				authdata = AuthData(password.c_str(), password.length());
				password_sha1_hash = authdata.getPasswordHash();

				command.clear();
				command = "INSERT INTO hash (user, uint1, uint2, uint3, uint4, uint5) VALUES (";
				command += results[0] + "," +
				           to_string(password_sha1_hash[0]) + "," +
				           to_string(password_sha1_hash[1]) + "," +
				           to_string(password_sha1_hash[2]) + "," +
				           to_string(password_sha1_hash[3]) + "," +
				           to_string(password_sha1_hash[4]) + ")";

				mysql_query(&mysql_, command.c_str());

				if (result_ = mysql_store_result(&mysql_))
					mysql_free_result(result_);
			}
		}
	}
	user_file_stream.close();
}

vector<string> SQLdataBase::getResultsFromSQL() {
	vector<string> results;
	string str;

	if (result_ = mysql_store_result(&mysql_)) {
		while (row_ = mysql_fetch_row(result_)) {
			for (int i = 0; i < mysql_num_fields(result_); i++) {
				str = row_[i];
				//cout << str << endl;
				//logger_->log(str);
				results.push_back(str);
			}
		}
	} else {
		*logger_ << mysql_error(&mysql_) << endl;
		//cout <<  mysql_error(&mysql_) << endl;
	}
	return results;
}

void SQLdataBase::createTables() {
	fstream commands_stream = fstream(startData.SQL_commands,
	                                  ios::in | ios::out);
	if (commands_stream)
		*logger_ << "Creating SQL tables" << endl;
	else
		*logger_ << "Error: Cant open SQL commands file" << endl;

	string commands;
	while (!commands_stream.eof()) {
		getline(commands_stream, commands, ';');

		if (commands.size() > 2) {
			mysql_query(&mysql_, commands.c_str());
			printResult();
		}
	}

	if (result_ = mysql_store_result(&mysql_))
		mysql_free_result(result_);

	commands_stream.close();
}

void SQLdataBase::printResult() {
	//Выводим все что есть в базе через цикл
	if (result_ = mysql_store_result(&mysql_)) {
		while (row_ = mysql_fetch_row(result_)) {
			for (int i = 0; i < mysql_num_fields(result_); i++) {
				//cout << row_[i] << "  ";
				*logger_ << row_[i] << endl;
			}
			//cout << endl;
		}
	} else {
		*logger_ << mysql_error(&mysql_) << endl;
		//cout <<  mysql_error(&mysql_) << endl;
	}
}

void SQLdataBase::readUsersFromDataBase() {
	*logger_ << "Reading users from database" << endl;
	string command;

	command = "SELECT users.login, hash.uint1, hash.uint2, hash.uint3, hash.uint4, hash.uint5 ";
	command += "FROM users JOIN hash ON users.id = hash.user";
	mysql_query(&mysql_, command.c_str());
	*logger_ << command << endl;

	vector<string> results = getResultsFromSQL();

	string login;
	uint* hash;
	AuthData authdata;

	for(int i = 0; i < results.size(); i+=6) {
		hash = new uint[5];

		login = results[i];
		hash[0] = stoul(results[i+1]);
		hash[1] = stoul(results[i+2]);
		hash[2] = stoul(results[i+3]);
		hash[3] = stoul(results[i+4]);
		hash[4] = stoul(results[i+5]);

		authdata = AuthData(hash);
		addUsers(pair<string, AuthData>(login,authdata));
	}
}

void SQLdataBase::readMessagesFromDataBase() {
	*logger_ << "Reading messages from database" << endl;
	string command;

	command = "SELECT fromUser.login, toUser.login, messages.text ";
	command += "FROM messages ";
	command += "JOIN users fromUser ON messages.fromUser = fromUser.id ";
	command += "JOIN users toUser ON messages.toUser = toUser.id";

	*logger_ << command << endl;
	mysql_query(&mysql_, command.c_str());

	vector<string> results = getResultsFromSQL();

	string from, to, text;
	for(int i = 0; i < results.size(); i+=3) {

		from = results[i];
		to = results[i+1];
		text = results[i+2];

		insertMessages(Message(from,to,text));
	}
}

void SQLdataBase::writeMessageInDataBase(const Message& message) {
	string command;
	string from, to;
	vector<string> results;

	command = "SELECT id FROM users WHERE login = '" + message.getFrom() +
	          "'";
	mysql_query(&mysql_, command.c_str());
	results = getResultsFromSQL();
	if (results.size()!=0) {
		from = results[0];
	}

	command = "SELECT id FROM users WHERE login = '" + message.getTo() +
	          "'";
	mysql_query(&mysql_, command.c_str());
	results = getResultsFromSQL();
	if (results.size()!=0) {
		to = results[0];
	}

	if (from.size()!=0 && to.size()!=0 && message.getText().size()!=0) {
		command = "INSERT INTO messages (fromUser,toUser,text) VALUES (";
		command += from + "," + to + ",'" + message.getText() + "')";
		*logger_ << command << endl;
		mysql_query(&mysql_, command.c_str());
	}

	if (result_ = mysql_store_result(&mysql_))
		mysql_free_result(result_);
}

void SQLdataBase::writeUserInDataBase(const std::string& login,
                                      const std::string& password) {
	string command;

	command = "INSERT INTO users (login) VALUES ('" + login + "')";
	mysql_query(&mysql_, command.c_str());

	AuthData authdata;
	uint* password_sha1_hash;

	command = "SELECT id FROM users WHERE login = '" + login + "'";
	mysql_query(&mysql_, command.c_str());

	vector<string> results;
	results = getResultsFromSQL();
	if (results.size()!=0) {

		authdata = AuthData(password.c_str(), password.length());
		password_sha1_hash = authdata.getPasswordHash();

		command.clear();
		command = "INSERT INTO hash (user, uint1, uint2, uint3, uint4, uint5) VALUES (";
		command += results[0] + "," +
		           to_string(password_sha1_hash[0]) + "," +
		           to_string(password_sha1_hash[1]) + "," +
		           to_string(password_sha1_hash[2]) + "," +
		           to_string(password_sha1_hash[3]) + "," +
		           to_string(password_sha1_hash[4]) + ")";

		*logger_ << command << endl;
		mysql_query(&mysql_, command.c_str());

		if (result_ = mysql_store_result(&mysql_))
			mysql_free_result(result_);
	}

}

std::unordered_map<std::string, AuthData>::iterator
SQLdataBase::findUser(const std::string& login) {
	auto user = users_.find(login);
	if (user == users_.end()) {
		throw Error("ERROR: Cant find login");
	}
	return user;
}

bool SQLdataBase::isUserExists(const std::string& login) {
	auto user = users_.find(login);

	if (user != users_.end()) {
		return true;
	}
	return false;
}

void SQLdataBase::addMessage(const std::string& from,
                             const std::string& to, const std::string& text) {
	messages_.emplace_back(Message(from, to, text));
}

std::string SQLdataBase::displayUsersAndMessages() {
	string message;

	message+="---------------------------------------------------\n";
	message+="                          CHAT\n";
	message+="---------------------------------------------------\n";
	message+="(type 'end' to exit)\n";
	message+="Users: [";

	char comma[3] = { '\0', ' ', '\0' };
	for (auto user : users_) {
		if (user.second.getOnline()) {
			message+=comma;
			message+=user.first;
			comma[0] = ',';
		}
	}

	message+="]\n";

	for (auto& mes : messages_) {
		if (mes.getTo() == server_name_ || mes.getTo() == user_name_) {
			message+= mes.getFrom() + "(to "
			          + mes.getTo() + "):" + mes.getText() + "\n";
		}
	}

	return message;
}

void SQLdataBase::setUserName(const std::string& login) {
	auto user = users_.find(login);
	user->second.setOnline(1);
	user_name_ = user->first;
}

void SQLdataBase::setServerName(const std::string& login) {
	auto user = findUser(login);
	user->second.setOnline(1);
	server_name_ = user->first;
}

std::string SQLdataBase::getServerName() {
	return server_name_;
}

std::string SQLdataBase::getUserName() {
	return user_name_;
}

std::string SQLdataBase::showPermissions(filesystem::perms p) {
	string message;
	message = ((p & fs::perms::owner_read) != fs::perms::none ? "r" :
	           "-");
	message +=  ((p & fs::perms::owner_write) != fs::perms::none ? "w" :
	             "-");
	message += ((p & fs::perms::owner_exec) != fs::perms::none ? "x" :
	            "-");
	message += ((p & fs::perms::group_read) != fs::perms::none ? "r" :
	            "-");
	message += ((p & fs::perms::group_write) != fs::perms::none ? "w" :
	            "-");
	message += ((p & fs::perms::group_exec) != fs::perms::none ? "x" :
	            "-");
	message += ((p & fs::perms::others_read) != fs::perms::none ? "r" :
	            "-");
	message += ((p & fs::perms::others_write) != fs::perms::none ? "w" :
	            "-");
	message += ((p & fs::perms::others_exec) != fs::perms::none ? "x" :
	            "-");
	return message;
}