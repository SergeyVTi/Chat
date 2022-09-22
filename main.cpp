
#include "Chat.h"

typedef std::pair<string,AuthData> user;

int main() {
	setlocale(LC_ALL, "");
	const string users_file { "users.txt" };
	const string messages_file { "messages.txt" };

	Chat chat(users_file, messages_file);

	chat.insertUsers(user("Vasya",AuthData("VasyaPassword",strlen("VasyaPassword"))),
	                 user("Masha",AuthData("MashaPassword",strlen("MashaPassword"))));

	chat.insertMessages(Message("Sergey","Vasya","Hi Vasya"),
	                    Message("Vasya","All","Hello world"),	                   
	                    Message("Vasya","Masha","Hi"),
	                    Message("Masha","Vasya","Hello"));

	chat.readUsersFromFile(users_file);
	chat.readMessagesFromFile(messages_file);

	while(chat.displayMenu()) {}

	return 0;
}
