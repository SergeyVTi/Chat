
#include "Server.h"

#include <iostream>
#include <algorithm>


constexpr size_t SHA1HASHLENGTHBYTES = 20;

Server::Server(const string& users_file,
               const string& messages_file) : Chat(users_file, messages_file) {
#if defined(_WIN64)
	Connection_ = make_unique<ServerWin64TCPIP>();
#elif defined LINUX
	Connection_ = make_unique<ServerLin64TCPIP>();
#endif
}
//
bool Server::displayMenu() {

	try {
		cout << endl;
		//displayMessages();

		size_t selection{};

		cout << endl
		     << "---------------------------------" << endl
		     << "1) Login 2) Registration 3) Exit" << endl
		     << "---------------------------------" << endl
		     << "---> ";
		cin >> selection;

		if (cin.fail()) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::infinity(), '\n');
		}

		switch (selection) {
			case 1:
				displayLoginMenu();
				break;

			case 2:
				displaySignupMenu();
				break;

			case 3:
				return false;
				break;

			default:
				throw Error("ERROR: input error");
				break;
		}


		if (Connection_->makeConnection() == 1)
			return false;

		string select = Connection_->reciveMessage();

		if (select=="1")
			displayLoginMenuForClient();
		else if (select=="2")
			displaySignupMenuForClient();
		else if (select=="3")
			return false;

		while (displayChat()) {}

		return false;

	} catch (const exception& e) {
		cout << e.what() << endl;
	}
	return true;
}


void Server::displayLoginMenu() {
	string login = getString("login");

	auto user = users_.find(login);

	if (user == users_.end())
		throw Error("ERROR: Cant find login");

	string password = getString("password");

	uint* hash = user->second.sha1(password.c_str(), password.length());

	uint* password_sha1_hash_ = user->second.getPasswordHash();

	if (memcmp(hash, password_sha1_hash_, SHA1HASHLENGTHBYTES) != 0)
		throw Error("ERROR: wrong password");

	user->second.setOnline(1);
	server_name_ = user->first;
	delete[] hash;

}

void Server::displaySignupMenuForClient() {
	auto user = users_.begin();
	string login, password;

	while(true) {
		Connection_->sendMessage("Enter login:\n");
		login = Connection_->reciveMessage();

		user = users_.find(login);
		if (user != users_.end())
			Connection_->sendMessage("ERROR: login already in use\n");
		else {
			Connection_->sendMessage("Enter password:\n");
			password = Connection_->reciveMessage();

			writeUserInFile(login, password);

			insertUsers(pair<string, AuthData>(login, AuthData(password.c_str(),
			                                   password.length())));

			user = users_.find(login);
			user->second.setOnline(1);
			user_name_ = user->first;
			break;
		}
	}

}

void Server::displayLoginMenuForClient() {
	auto user = users_.begin();
	string login;

	while(true) {
		Connection_->sendMessage("Enter login:\n");
		login = Connection_->reciveMessage();

		user = users_.find(login);

		if (user == users_.end())
			Connection_->sendMessage("ERROR: Cant find login\n");
		else
			break;
		login.clear();
	}

	string password;
	uint* hash;
	uint* password_sha1_hash;

	while(true) {
		Connection_->sendMessage("Enter password:\n");
		password = Connection_->reciveMessage();

		hash = user->second.sha1(password.c_str(), password.length());

		password_sha1_hash = user->second.getPasswordHash();

		if (memcmp(hash, password_sha1_hash, SHA1HASHLENGTHBYTES) != 0)
			Connection_->sendMessage("ERROR: wrong password\n");
		else
			break;
		password.clear();
	}

	user->second.setOnline(1);//////
	user_name_ = user->first;
	delete[] hash;

}
//
//
void Server::displaySignupMenu() {
	string login = getString("login");

	auto user = users_.find(login);
	if (user != users_.end())
		throw Error("ERROR: login already in use");

	string password = getString("password");

	writeUserInFile(login, password);

	insertUsers(pair<string, AuthData>(login, AuthData(password.c_str(),
	                                   password.length())));

	user = users_.find(login);
	user->second.setOnline(1);
	server_name_ = user->first;
}
//
bool Server::displayChat() {
	displayUsersAndMessages();
	string messageIn,messageOut,message;

	while(true) {
		messageIn = Connection_->reciveMessage();
		if (messageIn=="end")
			return false;
		message = user_name_ + "(to " +  server_name_  + "): "  +  messageIn +
		          "\n";
		cout <<  message;

		messages_.emplace_back(Message(user_name_, server_name_, messageIn));
		writeMessageInFile(Message(user_name_, server_name_, messageIn));
		messageIn.clear();

		messageOut=server_name_+"(to "+user_name_+"): ";
		cout << "--->";

//		cin.ignore(numeric_limits<streamsize>::max(), '\n');
//		getline(cin, messageIn);
		cin >> messageIn;

		messageOut+=messageIn + "\n";

		Connection_->sendMessage(messageOut);

		if (messageIn=="end")
			return false;

		messages_.emplace_back(Message(server_name_, user_name_, messageIn));
		writeMessageInFile(Message(server_name_, user_name_, messageIn));

	}

	return true;
}
//
void Server::displayUsersAndMessages() {
	string message;
	message+="---------------------------------------------------\n";
	message+="                          CHAT\n";
	message+="---------------------------------------------------\n";
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
	cout << message;
	Connection_->sendMessage(message);
}
//

