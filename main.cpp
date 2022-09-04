
#include "Chat.h"

typedef std::pair<string,AuthData> user;

int main() {
	setlocale(LC_ALL, "");

	Chat chat;

	chat.insertUsers(user("Sergey",AuthData("SergeyPassword",
	                                        strlen("SergeyPassword"))),
	                 user("Vasya",AuthData("VasyaPassword",strlen("VasyaPassword"))),
	                 user("Masha",AuthData("MashaPassword",strlen("MashaPassword"))));

	chat.insertMessages(Message("Sergey","Vasya","Hi Vasya"),
	                    Message("Vasya","All","Hello world"),
	                    Message("Vasya","Sergey","Hello Sergey"),
	                    Message("Vasya","Masha","Hi"),
	                    Message("Masha","Vasya","Hello"));

	while(chat.displayMenu()) {}

	return 0;
}
