
#include "Chat.h"

#include <iostream>
#include <algorithm>

constexpr size_t SHA1HASHLENGTHBYTES = 20;

Chat::Chat(const string& users_file, const string& messages_file) : 
	users_file_(users_file), messages_file_(messages_file) {
	
}

void Chat::show_perms(filesystem::perms p)
{
	std::cout << ((p & fs::perms::owner_read) != fs::perms::none ? "r" : "-")
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

void Chat::readMessagesFromFile(const string& file) {
	fstream messages_file_stream = fstream(messages_file_, ios::in | ios::out);
	if (!messages_file_stream) {
		messages_file_stream = fstream(messages_file_, ios::in | ios::out | ios::trunc);
		fs::permissions(messages_file_,fs::perms::all,fs::perm_options::remove);
		fs::permissions(messages_file_,
			fs::perms::owner_read | fs::perms::owner_write,
			fs::perm_options::replace);
		cout << "Created file <" << messages_file_ << "> with permissions: ";
		show_perms(fs::status(messages_file_).permissions());
	}
	else {
		fs::permissions(messages_file_, fs::perms::all, fs::perm_options::remove);
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



void Chat::readUsersFromFile(const string& file) {	
	fstream user_file_stream = fstream(users_file_, ios::in | ios::out);
	if (!user_file_stream) {
		user_file_stream = fstream(users_file_, ios::in | ios::out | ios::trunc);
		fs::permissions(messages_file_, fs::perms::all, fs::perm_options::remove);
		fs::permissions(users_file_,
			fs::perms::owner_read | fs::perms::owner_write,
			fs::perm_options::replace);
		cout << "Created file <" << users_file_ << "> with permissions: ";
		show_perms(fs::status(users_file_).permissions());
	}
	else {
		fs::permissions(messages_file_, fs::perms::all, fs::perm_options::remove);
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

			insertUsers(pair<string, AuthData>(login, AuthData(password.c_str(), password.length())));

			if (user_file_stream.eof())
				break;
		}
	}
	user_file_stream.close();
}

void Chat::writeMessageInFile(const Message& message) {
	fstream messages_file_stream = fstream(messages_file_, ios::in | ios::out | ios::ate);
	if (!messages_file_stream) {
		messages_file_stream = fstream(messages_file_, ios::in | ios::out | ios::trunc);
	}

	if (messages_file_stream) {
		messages_file_stream << endl 
			<< message.getFrom() << " " 
			<< message.getTo() << " "
			<< message.getText();
	}
	messages_file_stream.close();
}

void Chat::writeUserInFile(const string& login, const string& password) {
	fstream user_file_stream = fstream(users_file_, ios::in | ios::out | ios::ate);
	if (!user_file_stream) {
		user_file_stream = fstream(users_file_, ios::in | ios::out | ios::trunc);
	}

	if (user_file_stream) {
		user_file_stream << endl << login << " " << password;	
	}
	user_file_stream.close();
}

bool Chat::displayMenu() {

	try {
		cout << endl;
		displayMessages();

		size_t selection{};

		cout << endl
			<< "---------------------------------" << endl
			<< "1) Login 2) Registration 3) Exit" << endl
			<< "---------------------------------" << endl
			<< "---> ";
		cin >> selection;

		if (cin.fail()) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
	}
	catch (const exception& e) {
		cout << e.what() << endl;
	}
	return true;
}


void Chat::displayLoginMenu() {
	string login = getString("login");

	auto user = users_.find(login);

	if (user == users_.end())
		throw Error("ERROR: Cant find login");

	string password = getString("password");

	uint* hash = user->second.sha1(password.c_str(), password.length());

	uint* password_sha1_hash_ = user->second.getPasswordHash();

	if (memcmp(hash, password_sha1_hash_, SHA1HASHLENGTHBYTES) != 0)
		throw Error("ERROR: wrong password");

	delete[] hash;

	while (displayChat(*user)) {}

}


void Chat::displaySignupMenu() {
	string login = getString("login");

	auto user = users_.find(login);
	if (user != users_.end())
		throw Error("ERROR: login already in use");

	string password = getString("password");

	writeUserInFile(login, password);

	insertUsers(pair<string, AuthData>(login, AuthData(password.c_str(), password.length())));

	user = users_.find(login);

	while (displayChat(*user)) {}

}

bool Chat::displayChat(const pair<string, AuthData>& user) {
	displayUsers();

	displayMessages(user);

	try {
		size_t selection;
		cout << endl
			<< "------------------------------------------------------" << endl
			<< "1) send Message To All 2) send Message To User 3) Exit" << endl
			<< "------------------------------------------------------" << endl
			<< "---> ";
		cin >> selection;

		if (cin.fail()) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}

		switch (selection) {
		case 1:
			sentMessageToAll(user);
			break;

		case 2:
			sentMessageToUser(user);
			break;

		case 3:
			return false;

		default:
			throw Error("ERROR: input error");
			break;
		}
	}
	catch (const exception& e) {
		cout << e.what() << endl;
	}

	return true;
}



void Chat::displayUsers() {
	cout << "---------------------------------------------------" << endl
		<< "                          CHAT" << endl
		<< "---------------------------------------------------" << endl
		<< "Users: [";
	char comma[3] = { '\0', ' ', '\0' };
	for (auto user : users_) {
		if (user.first != "All") {
			cout << comma << user.first;
			comma[0] = ',';
		}
	}
	cout << ']' << endl;
}

void Chat::displayMessages(const pair<string, AuthData>& to) {
	for (auto& mes : messages_) {
		if (mes.getTo() == to.first || mes.getTo() == "All"
			|| mes.getFrom() == to.first) {
			cout << mes.getFrom() << "(to "
				<< mes.getTo() << "): " << mes.getText()
				<< endl;
		}
	}
}

void Chat::displayMessages() {
	for (auto& mes : messages_) {
		if (mes.getTo() == "All") {
			cout << mes.getFrom() << "(to "
				<< mes.getTo() << "): " << mes.getText()
				<< endl;
		}
	}
}

void Chat::sentMessageToAll(const pair<string, AuthData>& from) {
	string message;
	cout << "---> ";
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	getline(cin, message);

	messages_.emplace_back(Message(from.first, "All", message));
	writeMessageInFile(Message(from.first, "All", message));
}

string Chat::getString(const string& str) {
	string login;
	cout << endl
		<< "Enter " << str << endl
		<< "---> ";
	cin >> login;

	return login;
}

void Chat::sentMessageToUser(const pair<string, AuthData>& from) {
	string name = getString("user name");

	auto user = users_.find(name);

	if (user == users_.end())
		throw Error("ERROR: Cant find user");

	string message;
	cout << "---> ";
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	getline(cin, message);

	messages_.emplace_back(Message(from.first, user->first, message));
	writeMessageInFile(Message(from.first, user->first, message));

	return;

}

