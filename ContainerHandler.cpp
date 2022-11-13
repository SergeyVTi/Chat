#include "FileIO.h"

#include <iostream>
#include <cstring>

namespace fs = std::filesystem;
using namespace std;

unordered_map<std::string, AuthData>::iterator
ContainerHandler::findUser(const std::string& login) {
	auto user = users_.find(login);
	if (user == users_.end())
		throw Error("ERROR: Cant find login");

	return user;
}

bool ContainerHandler::isUserExists(const string& login) {
	auto user = users_.begin();
	user = users_.find(login);

	if (user != users_.end()) {
		return true;
	}
	return false;
}

void ContainerHandler::setUserName(const string& login) {
	auto user = users_.find(login);
	user->second.setOnline(1);
	user_name_ = user->first;
}

void ContainerHandler::setServerName(const string& login) {
	auto user = findUser(login);
	user->second.setOnline(1);
	server_name_ = user->first;
}

std::string ContainerHandler::getServerName() {
	return server_name_;
}

std::string ContainerHandler::getUserName() {
	return user_name_;
}

void ContainerHandler::showPermissions(filesystem::perms p) {
	std::cout << ((p & fs::perms::owner_read) != fs::perms::none ? "r" :
	              "-")
	          << ((p & fs::perms::owner_write) != fs::perms::none ? "w" : "-")
	          << ((p & fs::perms::owner_exec) != fs::perms::none ? "x" : "-")
	          << ((p & fs::perms::group_read) != fs::perms::none ? "r" : "-")
	          << ((p & fs::perms::group_write) != fs::perms::none ? "w" : "-")
	          << ((p & fs::perms::group_exec) != fs::perms::none ? "x" : "-")
	          << ((p & fs::perms::others_read) != fs::perms::none ? "r" : "-")
	          << ((p & fs::perms::others_write) != fs::perms::none ? "w" : "-")
	          << ((p & fs::perms::others_exec) != fs::perms::none ? "x" : "-")
	          << '\n';
}

void ContainerHandler::readMessagesFromFile() {
	fstream messages_file_stream = fstream(messages_file_,
	                                       ios::in | ios::out);
	if (!messages_file_stream) {
		messages_file_stream = fstream(messages_file_,
		                               ios::in | ios::out | ios::trunc);
		fs::permissions(messages_file_,fs::perms::all,
		                fs::perm_options::remove);
		fs::permissions(messages_file_,
		                fs::perms::owner_read | fs::perms::owner_write,
		                fs::perm_options::replace);
		cout << "Created file <" << messages_file_ << "> with permissions: ";
		showPermissions(fs::status(messages_file_).permissions());
	} else {
		fs::permissions(messages_file_, fs::perms::all,
		                fs::perm_options::remove);
		fs::permissions(messages_file_,
		                fs::perms::owner_read | fs::perms::owner_write,
		                fs::perm_options::replace);
		cout << "Opened file <" << messages_file_ << "> with permissions: ";
		showPermissions(fs::status(messages_file_).permissions());
	}

	if (messages_file_stream) {
		string from, to, text;

		while (!messages_file_stream.eof()) {

			messages_file_stream >> from >> to;

			getline(messages_file_stream, text, '\n');

			messages_.emplace_back(Message(from, to, text));

			if (messages_file_stream.eof())
				break;
		}
	}
	messages_file_stream.close();
}

void ContainerHandler::readUsersFromFile() {
	addUsers(pair<string, AuthData>("Vasya", AuthData("VasyaPassword",
	                                strlen("VasyaPassword"))),
	         pair<string, AuthData>("Masha", AuthData("MashaPassword",
	                                strlen("MashaPassword"))),
	         pair<string, AuthData>("Sergey", AuthData("SergeyPassword",
	                                strlen("SergeyPassword"))));

	insertMessages(Message("Sergey", "Vasya", "Hi Vasya"),
	               Message("Vasya", "All", "Hello world"),
	               Message("Vasya", "Masha", "Hi"),
	               Message("Masha", "Vasya", "Hello"));

	fstream user_file_stream = fstream(users_file_, ios::in | ios::out);
	if (!user_file_stream) {
		user_file_stream = fstream(users_file_,
		                           ios::in | ios::out | ios::trunc);
		fs::permissions(users_file_, fs::perms::all,
		                fs::perm_options::remove);
		fs::permissions(users_file_,
		                fs::perms::owner_read | fs::perms::owner_write,
		                fs::perm_options::replace);
		cout << "Created file <" << users_file_ << "> with permissions: ";
		showPermissions(fs::status(users_file_).permissions());

		writeUserInFile("All","AllPassword");
	} else {
		fs::permissions(users_file_, fs::perms::all,
		                fs::perm_options::remove);
		fs::permissions(users_file_,
		                fs::perms::owner_read | fs::perms::owner_write,
		                fs::perm_options::replace);
		cout << "Opened file <" << users_file_ << "> with permissions: ";
		showPermissions(fs::status(users_file_).permissions());

	}

	if (user_file_stream) {
		string login, password;

		while (!user_file_stream.eof()) {

			user_file_stream >> login >> password;

			addUsers(pair<string, AuthData>(login, AuthData(password.c_str(),
			                                password.length())));
		}
	}
	user_file_stream.close();
}

void ContainerHandler::writeMessageInFile(const Message& message) {
	fstream messages_file_stream = fstream(messages_file_,
	                                       ios::in | ios::out | ios::ate);
	if (!messages_file_stream) {
		messages_file_stream = fstream(messages_file_,
		                               ios::in | ios::out | ios::trunc);
	}

	if (messages_file_stream) {
		messages_file_stream << endl
		                     << message.getFrom() << " "
		                     << message.getTo() << " "
		                     << message.getText();
	}
	messages_file_stream.close();
}

void ContainerHandler::writeUserInFile(const string& login,
                                       const string& password) {
	fstream user_file_stream = fstream(users_file_,
	                                   ios::in | ios::out | ios::ate);
	if (!user_file_stream) {
		user_file_stream = fstream(users_file_,
		                           ios::in | ios::out | ios::trunc);
	}

	if (user_file_stream) {
		user_file_stream << endl << login << " " << password;
	}
	user_file_stream.close();
}

void ContainerHandler::addMessage(const string& from, const string& to
                                  , const string& text) {
	messages_.emplace_back(Message(from, to, text));
}

string ContainerHandler::displayUsersAndMessages() {
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

	return message;
}