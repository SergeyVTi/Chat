
#include "Chat.h"

#if defined(_WIN32)
#define PLATFORM_NAME "Windows 32-bit" // Windows 32-bit
#elif defined(_WIN64)
#define PLATFORM_NAME "Windows 64-bit" // Windows 64-bit
#elif defined(__linux__) // Linux
#define PLATFORM_NAME "Linux"
#include <sys/utsname.h> 
#else
#define PLATFORM_NAME "OS not detected"
#endif

typedef std::pair<string, AuthData> user;

int main()
{
	setlocale(LC_ALL, "");

	#if defined(__linux__)
    struct utsname utsname; 
    uname(&utsname); 
    
    cout << "OS name: " << utsname.sysname << " " << utsname.machine << endl;    
    cout << "OS version: " << utsname.version << endl;
    #elif defined(_WIN32)
	cout << "OS version: "<< PLATFORM_NAME << endl;
	#elif defined(_WIN64)
	cout << "OS version: "<< PLATFORM_NAME << endl;
	#else
	cout << "OS version: "<< PLATFORM_NAME << endl;
	#endif
	
	const string users_file{"users.txt"};
	const string messages_file{"messages.txt"};

	Chat chat(users_file, messages_file);

	chat.insertUsers(user("Vasya", AuthData("VasyaPassword", strlen("VasyaPassword"))),
					 user("Masha", AuthData("MashaPassword", strlen("MashaPassword"))));

	chat.insertMessages(Message("Sergey", "Vasya", "Hi Vasya"),
						Message("Vasya", "All", "Hello world"),
						Message("Vasya", "Masha", "Hi"),
						Message("Masha", "Vasya", "Hello"));

	chat.readUsersFromFile();
	chat.readMessagesFromFile();

	while (chat.displayMenu())
	{
	}

	return 0;
}
