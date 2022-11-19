#pragma once
#include "Chat.h"

#if defined(__linux__)
#define LINUX
#else
#define LINUX
#endif

class Server : public Chat {
	public:
		Server(const StartData& data) : Chat(data) {}
		virtual ~Server() = default;

		virtual void displayMenu() override;
		virtual void startChat() override;
		virtual void setDataBase() override;
		virtual	void makeConnection() override;

	protected:
		bool displayChat();
		void displayLoginMenu();
		void displaySignupMenu();
		void displayLoginMenuForClient();
		void displaySignupMenuForClient();
		void displayUsersAndMessages();
		void displayMenuForClient();
		bool isCorrectPassword(const std::string& password,
		                       const std::string& login);

		void readUsersFromDataBase();
		void readMessagesFromDataBase();
		bool isConnectedToSQLdataBase();

	protected:

};
