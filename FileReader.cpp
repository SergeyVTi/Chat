#include "FileIO.h"

#include <iostream>
#include <cstring>

using namespace std;

namespace fs = std::filesystem;

void FileReader::showPermissions(filesystem::perms p) {
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

void FileReader::readMessagesFromFile() {
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



void FileReader::readUsersFromFile() {
//	insertUsers(pair<string, AuthData>("Vasya", AuthData("VasyaPassword",
//	                                   strlen("VasyaPassword"))),
//	            pair<string, AuthData>("Masha", AuthData("MashaPassword",
//	                                   strlen("MashaPassword"))));
//
//	insertMessages(Message("Sergey", "Vasya", "Hi Vasya"),
//	               Message("Vasya", "All", "Hello world"),
//	               Message("Vasya", "Masha", "Hi"),
//	               Message("Masha", "Vasya", "Hello"));

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

	//	writeUserInFile("All","AllPassword");
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

		//	insertUsers(pair<string, AuthData>(login, AuthData(password.c_str(),
		//	                                   password.length())));

			if (user_file_stream.eof())
				break;
		}
	}
	user_file_stream.close();
}