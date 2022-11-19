
#include "Chat.h"
#include "Client.h"
#include "Server.h"
#include "InOut.h"
#include "OS_info.h"

using namespace std;

unique_ptr<Chat> selectChatMode();
StartData startData;

int main() {
	setlocale(LC_ALL, "");
	OS os;

	try {
		os.displayVersion();

		unique_ptr<Chat> chat = selectChatMode();

		chat->setDataBase();

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
					return make_unique<Server>(startData);

				case 2:
//					if (ip_adr.size() == 0)
//						ip_adr = input.getInputString("server ip adress");

					return make_unique<Client>(startData);

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
