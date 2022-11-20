
#include "Server.h"
#include "InOut.h"
#include "Errors.h"
#include "SQLdataBase.h"

#include <iostream>
#include <algorithm>

using namespace std;

constexpr size_t SHA1HASHLENGTHBYTES = 20;

void Server::makeConnection() {
#if defined(_WIN64)
	connection_ = make_unique<ServerWinTCP>();
#elif defined LINUX
	connection_ = make_unique<ServerLinuxTCP>();
#endif

	if (connection_->makeConnection() == 1)
		throw Error("ERROR: connection fail");
}

void Server::setDataBase() {
	containerHandler_ = make_unique<SQLdataBase>(startData);

	if (isConnectedToSQLdataBase()) {
		//cout << "Using SQL database" << endl;

	} else {
		containerHandler_ = make_unique<DefaultHandler>
		                    (startData.users_file,
		                     startData.messages_file);

		cout << "Using default database" << endl;
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

	string menu = output.getLoginMenu();
	size_t selection {};

	while(true) {
		try {
			cout << menu;

			selection = input.getInputSelection();

			switch (selection) {
				case 1:
					displayLoginMenu();
					break;

				case 2:
					displaySignupMenu();
					break;

				case 3:
					throw Exit();

				default:
					throw Error("ERROR: input error");
			}

			if (successfulAccessToAccount_)
				break;

		} catch (const Error& e) {
			cout << e.what() << endl;
		}
	}
}

void Server::displayLoginMenu() {
	string login = input.getInputString("login");

	auto user = findUser(login);

	string password = input.getInputString("password");

	uint* hash = user->second.sha1(password.c_str(), password.length());

	uint* password_sha1_hash_ = user->second.getPasswordHash();

	if (memcmp(hash, password_sha1_hash_, SHA1HASHLENGTHBYTES) != 0)
		throw Error("ERROR: wrong password");

	setServerName(login);

	delete[] hash;

	successfulAccessToAccount_ = 1;
}

void Server::displaySignupMenuForClient() {
	string login, password;

	while(true) {
		sendMessage("Enter login:\n");
		login = reciveMessage();

		if (isUserExists(login))
			sendMessage("ERROR: login already in use\n");
		else {
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
//
void Server::displaySignupMenu() {
	string login = input.getInputString("login");
	if (isUserExists(login))
		throw Error("ERROR: login already in use");

	string password = input.getInputString("password");

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
	string messageIn;
	size_t selection;

	string menu = output.getLoginMenu();

	while (true) {
		sendMessage(menu);

		messageIn = reciveMessage();
		selection = messageIn[0];

		switch (selection) {
			case '1':
				displayLoginMenuForClient();
				return;

			case '2':
				displaySignupMenuForClient();
				return;

			case '3':
				sendMessage("end");
				throw Exit();

			default:
				sendMessage("ERROR: input error");
				break;
		}
	}
}

bool Server::displayChat() {
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


