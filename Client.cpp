
#include "Client.h"

#include <iostream>
#include <algorithm>
#include <limits>

constexpr size_t SHA1HASHLENGTHBYTES = 20;

Client::Client(const string& users_file,
               const string& messages_file,
               const string& ip_file) : Chat(users_file, messages_file) {
#if defined(_WIN64)
	Connection_ = make_unique<ClientWin64TCPIP>(ip_file);
#elif defined LINUX
	Connection_ = make_unique<ClientLin64TCPIP>(ip_file);
#endif
}
//
bool Client::displayMenu() {

	if (Connection_->makeConnection() == 1)
		return false;

	try {
		cout << endl;

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
				Connection_->sendMessage("1");
				break;

			case 2:
				Connection_->sendMessage("2");
				break;

			case 3:
				return false;
				break;

			default:
				throw Error("ERROR: input error");
				break;
		}

		string messageOut, messageIn;
		while(true) {
			messageIn = Connection_->reciveMessage();

			if (messageIn=="end")
				return false;

			cout << messageIn;

			cout << "--->";
//			cin.ignore(numeric_limits<streamsize>::max(), '\n');
//			getline(cin, message);
			cin >> messageOut;
			Connection_->sendMessage(messageOut);
			
			if (messageOut=="end")
				return false;
		}

	} catch (const exception& e) {
		cout << e.what() << endl;
	}
	return true;
}




