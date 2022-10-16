
#include "Chat.h"

#include <iostream>
#include <algorithm>
using namespace std;
constexpr size_t SHA1HASHLENGTHBYTES = 20;

Chat::Chat(const string& users_file, const string& messages_file) :
	users_file_(users_file), messages_file_(messages_file) {

}

void Chat::show_perms(filesystem::perms p) {
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

void Chat::readMessagesFromFile() {
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
		show_perms(fs::status(messages_file_).permissions());
	} else {
		fs::permissions(messages_file_, fs::perms::all,
		                fs::perm_options::remove);
		fs::permissions(messages_file_,
		                fs::perms::owner_read | fs::perms::owner_write,
		                fs::perm_options::replace);
		cout << "Opened file <" << messages_file_ << "> with permissions: ";
		show_perms(fs::status(messages_file_).permissions());
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



void Chat::readUsersFromFile() {
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
		show_perms(fs::status(users_file_).permissions());

		writeUserInFile("All","AllPassword");
	} else {
		fs::permissions(users_file_, fs::perms::all,
		                fs::perm_options::remove);
		fs::permissions(users_file_,
		                fs::perms::owner_read | fs::perms::owner_write,
		                fs::perm_options::replace);
		cout << "Opened file <" << users_file_ << "> with permissions: ";
		show_perms(fs::status(users_file_).permissions());

	}


	if (user_file_stream) {
		string login, password;

		while (!user_file_stream.eof()) {

			user_file_stream >> login >> password;

			insertUsers(pair<string, AuthData>(login, AuthData(password.c_str(),
			                                   password.length())));

			if (user_file_stream.eof())
				break;
		}
	}
	user_file_stream.close();
}

void Chat::writeMessageInFile(const Message& message) {
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

void Chat::writeUserInFile(const string& login,
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
//
string Chat::getString(const string& str) {
	string login;
	cout << endl
	     << "Enter " << str << endl
	     << "---> ";
	cin >> login;

	return login;
}
//

