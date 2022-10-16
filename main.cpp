
#include "Chat.h"
#include "Client.h"
#include "Server.h"

#if defined(_WIN64)
#define PLATFORM_NAME "Windows 64-bit" // Windows 64-bit
#elif defined(__linux__) // Linux
#define PLATFORM_NAME "Linux"
#define LINUX
#include <sys/utsname.h>
#else
#define LINUX
#define PLATFORM_NAME "OS not detected"
#endif

size_t displayMenu();

typedef std::pair<string, AuthData> user;

int main() {
	setlocale(LC_ALL, "");

#if defined (__linux__)
	struct utsname utsname;
	uname(&utsname);

	cout << "OS name: " << utsname.sysname << " " << utsname.machine <<
	     endl;
	cout << "OS version: " << utsname.version << endl;
	cout << "OS version: "<< PLATFORM_NAME << endl;
#else
	cout << "OS version: "<< PLATFORM_NAME << endl;
#endif

	const string users_file{"users.txt"};
	const string messages_file{"messages.txt"};
	string ip_adr{""};//

	size_t selection = displayMenu();
	unique_ptr<Chat> chat;

	if (selection==1)
		chat = make_unique<Server>(users_file, messages_file);
	else if (selection==2) {
		if (ip_adr.size()==0) {
			cout << "Enter server ip adress:" << endl
			     << "--->";
			cin >> ip_adr;
		}
		chat = make_unique<Client>(users_file, messages_file, ip_adr);
	}


	chat->insertUsers(user("Vasya", AuthData("VasyaPassword",
	                       strlen("VasyaPassword"))),
	                  user("Masha", AuthData("MashaPassword", strlen("MashaPassword"))));

	chat->insertMessages(Message("Sergey", "Vasya", "Hi Vasya"),
	                     Message("Vasya", "All", "Hello world"),
	                     Message("Vasya", "Masha", "Hi"),
	                     Message("Masha", "Vasya", "Hello"));

	chat->readUsersFromFile();
	chat->readMessagesFromFile();

	while (chat->displayMenu()) {
	}

	return 0;
}

size_t displayMenu() {
	size_t selection{};

	cout << endl
	     << "Select mode:" << endl
	     << "1) Server 2) Client " << endl
	     << "---------------------------------" << endl
	     << "---> ";
	cin >> selection;
	return selection;
}
