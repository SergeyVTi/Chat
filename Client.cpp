
#include "Client.h"

#include <iostream>
#include <algorithm>
#include <limits>

using namespace std;

constexpr size_t SHA1HASHLENGTHBYTES = 20;

Client::Client(const string& users_file,
               const string& messages_file,
               const string& ip_file) : Chat(users_file, messages_file) {
#if defined(_WIN64)
	connection_ = make_unique<ClientWinTCP>(ip_file);
#elif defined LINUX
	connection_ = make_unique<ClientLinuxTCP>(ip_file);
#endif
}

void Client::displayMenu() {

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



