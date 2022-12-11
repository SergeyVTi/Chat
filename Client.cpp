
#include "Client.h"

#include <iostream>
#include <algorithm>
#include <limits>

using namespace std;

constexpr size_t SHA1HASHLENGTHBYTES = 20;

void Client::makeConnection() {
#if defined(_WIN64)
	connection_ = make_unique<ClientWinTCP>(startData_.ip_adr);
#elif defined LINUX
	connection_ = make_unique<ClientLinuxTCP>(startData_.ip_adr);
#endif

	if (connection_->makeConnection() == 1)
		throw Error("ERROR: connection fail");
}

void Client::displayMenu() {
}

void Client::setDataBase() {
}

void Client::startChat() {
	string messageIn, messageOut;

	while(true) {
		messageIn = reciveMessage();

		if (messageIn == "end")
			throw Exit();

		cout << messageIn;

		cout << "--->";
		cin >> messageOut;

		sendMessage(messageOut);
	}
}



