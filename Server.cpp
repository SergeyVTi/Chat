
#include "Server.h"
#include "InOut.h"
#include "Errors.h"
#include "SQLdataBase.h"

#include <iostream>
#include <algorithm>
#include <memory>

using namespace std;

constexpr size_t SHA1HASHLENGTHBYTES = 20;

void Server::makeConnection() {
#if defined(_WIN64)
	*logger_ << "Making ServerWinTCP connection" << endl;
	connection_ = make_unique<ServerWinTCP>();
#elif defined LINUX
	*logger_ << "Making ServerLinuxTCP connection" << endl;
	connection_ = make_unique<ServerLinuxTCP>();
#endif

	connection_->setLogger(logger_);

	if (connection_->makeConnection() == 1) {
		*logger_ << "ERROR: connection fail" << endl;
		throw Error("ERROR: connection fail");
	}

	*logger_ << "Connection making done" << endl;
}

void Server::setDataBase() {
	containerHandler_ = make_unique<SQLdataBase>();

	containerHandler_->setLogger(logger_);

	if (isConnectedToSQLdataBase()) {
		*logger_ << "Using SQL database" << endl;
	} else {
		*logger_ << "ERROR: cant connect to SQL database" << endl;
		throw Error("ERROR: cant connect to SQL database");
	}

	readUsersFromDataBase();
	readMessagesFromDataBase();
}

bool  Server::isConnectedToSQLdataBase() {
	return containerHandler_->isConnectedToSQLdataBase();
}

void Server::readUsersFromDataBase() {
	containerHandler_->readUsersFromDataBase();
}

void Server::readMessagesFromDataBase() {
	containerHandler_->readMessagesFromDataBase();
}

void Server::displayMenu() {
	*logger_ << "Server displayLoginMenu" << endl;

	string menu = output_.getLoginMenu();
	size_t selection {};

	while(true) {
		try {
			cout << menu;

			selection = input_.getInputSelection();

			if (selection == 1) {
				displayLoginMenu();
			} else if (selection == 2) {
				displaySignupMenu();
			} else if (selection == 3) {
				throw Exit();
			} else {
				throw Error("ERROR: input_ error");
			}

			if (successfulAccessToAccount_)
				break;
		} catch (const Error& e) {
			cout << e.what() << endl;
		}
	}
}

void Server::displayLoginMenu() {
	*logger_ << "Server displayLoginMenu" << endl;

	string login = input_.getInputString("login");

	auto user = findUser(login);

	string password = input_.getInputString("password");

	uint* hash = user->second.sha1(password.c_str(), password.length());

	uint* password_sha1_hash_ = user->second.getPasswordHash();

	if (memcmp(hash, password_sha1_hash_, SHA1HASHLENGTHBYTES) != 0) {
		throw Error("ERROR: wrong password");
	}

	setServerName(login);

	delete[] hash;

	successfulAccessToAccount_ = 1;
}

void Server::displaySignupMenuForClient() {
	*logger_ << "Server displaySignupMenuForClient" << endl;
	string login, password;

	while(true) {
		sendMessage("Enter login:\n");
		login = reciveMessage();

		if (isUserExists(login)) {
			sendMessage("ERROR: login already in use\n");
		} else {
			sendMessage("Enter password:\n");
			password = reciveMessage();

			writeUserInDataBase(login, password);

			addUsers(pair<string, AuthData>(login, AuthData(password.c_str(),
			                                password.length())));

			setUserName(login);
			break;
		}
	}
}

void Server::displayLoginMenuForClient() {
	*logger_ << "Server displayLoginMenuForClient" << endl;
	string login;

	while(true) {
		sendMessage("Enter login:\n");

		login = reciveMessage();

		if (isUserExists(login))
			break;
	}

	string password;

	while(true) {
		sendMessage("Enter password:\n");

		password = reciveMessage();

		if (isCorrectPassword(password, login))
			break;
	}

	setUserName(login);
}

bool Server::isCorrectPassword(const string& password,
                               const string& login) {
	*logger_ << "Server isCorrectPassword" << endl;

	auto user = findUser(login);

	uint* hash;
	uint* password_sha1_hash;
	hash = user->second.sha1(password.c_str(), password.length());
	password_sha1_hash = user->second.getPasswordHash();
	if (memcmp(hash, password_sha1_hash, SHA1HASHLENGTHBYTES) == 0) {
		delete[] hash;
		return true;
	}

	delete[] hash;
	return false;
}

void Server::displaySignupMenu() {
	*logger_ << "Server displaySignupMenu" << endl;

	string login = input_.getInputString("login");
	if (isUserExists(login))
		throw Error("ERROR: login already in use");

	string password = input_.getInputString("password");

	writeUserInDataBase(login, password);

	addUsers(pair<string, AuthData>(login, AuthData(password.c_str(),
	                                password.length())));

	setServerName(login);

	successfulAccessToAccount_ = 1;
}

void Server::startChat() {
	displayMenuForClient();
	displayChat();
}

void Server::displayMenuForClient() {
	*logger_ << "Server displayMenuForClient" << endl;

	string messageIn;
	size_t selection;

	string menu = output_.getLoginMenu();

	while (true) {
		sendMessage(menu);

		messageIn = reciveMessage();
		selection = messageIn[0];

		if (selection == '1') {
			displayLoginMenuForClient();
			return;
		} else if (selection == '2') {
			displaySignupMenuForClient();
			return;
		} else if (selection == '3') {
			sendMessage("end");
			throw Exit();
		} else {
			sendMessage("ERROR: input error");
		}
	}
}

bool Server::displayChat() {
	*logger_ << "Server displayChat" << endl;
	displayUsersAndMessages();

	string messageIn, messageOut, message;
	string user_name = getUserName();
	string server_name = getServerName();

	while(true) {
		messageIn = reciveMessage();

		if (messageIn=="end") {
			sendMessage("end");
			throw Exit();
		}

		message = user_name + "(to " +  server_name  + "):"  +  messageIn +
		          "\n";
		cout <<  message;

		addMessage(user_name, server_name, messageIn);
		writeMessageInDataBase(Message(user_name, server_name, messageIn));

		messageIn.clear();

		messageOut=server_name+"(to "+user_name+"):";
		cout << "--->";

		cin >> messageIn;

		if (messageIn=="end") {
			sendMessage("end");
			throw Exit();
		}

		messageOut+=messageIn + "\n";

		sendMessage(messageOut);

		addMessage(server_name, user_name, messageIn);

		writeMessageInDataBase(Message(server_name, user_name, messageIn));
	}

	return true;
}
//
void Server::displayUsersAndMessages() {
	string message = containerHandler_->displayUsersAndMessages();

	cout << message;

	sendMessage(message);
}


