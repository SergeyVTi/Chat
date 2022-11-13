
#include "Chat.h"
#include "Client.h"
#include "Server.h"
#include "InOut.h"
#include "OS_info.h"

using namespace std;

void displayOSversion();
unique_ptr<Chat> selectChatMode();

const string users_file = "users.txt";
const string messages_file = "messages.txt";
string ip_adr = "127.0.0.1";

int main() {
	setlocale(LC_ALL, "");
	OS os;
	try {
		os.displayVersion();

		unique_ptr<Chat> chat = selectChatMode();

		chat->readUsersFromFile();

		chat->readMessagesFromFile();

		chat->makeConnection();

		chat->displayMenu();

		chat->startChat();

	} catch (const Exit& e) {
		cout << e.what() << endl;
	} catch (const exception& e) {
		cout << e.what() << endl;
	}
	return 0;
}


unique_ptr<Chat> selectChatMode() {
	Input input;
	Output output;
	size_t selection {};

	while(true) {
		try {
			cout << output.getSelectModeMenu();

			selection = input.getInputSelection();

			switch (selection) {
				case 1:
					return make_unique<Server>(users_file, messages_file);

				case 2:
					if (ip_adr.size() == 0) {
						cout << "Enter server ip adress:" << endl
						     << "--->";
						cin >> ip_adr;
					}

					return make_unique<Client>(users_file, messages_file, ip_adr);

				case 3:
					throw Exit();

				default:
					throw Error("ERROR: input error");				
			}
		} catch (const Error& e) {
			cout << e.what() << endl;
		}
	}
	
	return nullptr;
}
