#include "SQLdataBase.h"

using namespace std;

bool SQLdataBase::isConnectedToSQLdataBase() {
	mysql_init(&mysql_);
	if (&mysql_ == NULL) {
		// Если дескриптор не получен — выводим сообщение об ошибке
		cout << "Error: can't create MySQL-descriptor" << endl;
		return false;
	}
	// Подключаемся к серверу
	if (!mysql_real_connect(&mysql_, "localhost", startData_.SQL_user,
	                        startData_.SQL_password, startData_.SQL_name, 0, NULL, 0)) {
		// Если нет возможности установить соединение с БД выводим сообщение об ошибке
		cout << "Error: can't connect to database " << mysql_error(
		         &mysql_) << endl;
		return false;
	} else {
		// Если соединение успешно установлено
		cout << "Connected to SQL database " << startData_.SQL_name << "!" <<
		     endl;
	}

	mysql_set_character_set(&mysql_, "utf8");

	createTables();
	addUsersToSQLFromFile();
	return true;
}

void SQLdataBase::addUsersToSQLFromFile() {
	fstream user_file_stream = fstream(startData_.users_file,
	                                   ios::in | ios::out);

	string login, password;
	AuthData authdata;
	uint* password_sha1_hash;
	string command;
	vector<string> results;

	if (user_file_stream) {

		while (!user_file_stream.eof()) {

			user_file_stream >> login >> password;

			command = "INSERT INTO users (login) VALUES ('" + login + "')";
			mysql_query(&mysql_, command.c_str());

			command = "SELECT id FROM users WHERE login = '" + login + "'";
			mysql_query(&mysql_, command.c_str());

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
				results.push_back(str);
			}
		}
	} else {
		cout <<  mysql_error(&mysql_) << endl;
	}

	return results;
}

void SQLdataBase::createTables() {
	fstream commands_stream = fstream(startData_.SQL_commands,
	                                  ios::in | ios::out);
	if (commands_stream)
		cout << "Creating SQL tables:" << endl;

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
				cout << row_[i] << "  ";
			}
			cout << endl;
		}
	} else {
		cout <<  mysql_error(&mysql_) << endl;
	}
}

void SQLdataBase::readUsersFromDataBase() {
	string command;

	command = "SELECT users.login, hash.uint1, hash.uint2, hash.uint3, hash.uint4, hash.uint5 ";
	command += "FROM users JOIN hash ON users.id = hash.user";
	mysql_query(&mysql_, command.c_str());

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
		addUsers(pair<string,AuthData>(login,authdata));
	}
}

void SQLdataBase::readMessagesFromDataBase() {
	string command;

	command = "SELECT fromUser.login, toUser.login, messages.text ";
	command += "FROM messages ";
	command += "JOIN users fromUser ON messages.fromUser = fromUser.id ";
	command += "JOIN users toUser ON messages.toUser = toUser.id";

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

		mysql_query(&mysql_, command.c_str());

		if (result_ = mysql_store_result(&mysql_))
			mysql_free_result(result_);
	}

}

std::unordered_map<std::string, AuthData>::iterator
SQLdataBase::findUser(
    const std::string& login) {
	auto user = users_.find(login);
	if (user == users_.end())
		throw Error("ERROR: Cant find login");

	return user;
}

bool SQLdataBase::isUserExists(const std::string& login) {
	auto user = users_.begin();
	user = users_.find(login);

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