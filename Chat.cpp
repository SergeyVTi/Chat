
#include "Chat.h"

#include <iostream>
#include <algorithm>


constexpr size_t SHA1HASHLENGTHBYTES = 20;

Chat::Chat() {
	users_.insert({ "All",AuthData("AllPassword",strlen("AllPassword")) });
}

bool Chat::displayMenu() {

	try {
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

	users_.insert({ login,AuthData(password.c_str(),password.length()) });

	user = users_.find(login);

	while (displayChat(*user)) {}

}

bool Chat::displayChat(const pair<string, AuthData>& user) {
	displayUsers();

	displayMessages(user);

	try {
		size_t selection;
		cout << endl
			<< "---------------------------------------------------" << endl
			<< "1) send Message To All 2) send Message To User 3) Exit" << endl
			<< "---------------------------------------------------" << endl
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

void Chat::sentMessageToAll(const pair<string, AuthData>& from) {
	string message;
	cout << "---> ";
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	getline(cin, message);

	messages_.emplace_back(Message(from.first, "All", message));
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

	return;

}

