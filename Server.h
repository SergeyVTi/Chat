#pragma once
#include "Chat.h"

#if defined(__linux__)
#define LINUX
#else
#define LINUX
#endif

class Server : public Chat {
	public:
		Server(const std::string& users_file, const std::string& messages_file);
		virtual ~Server() = default;

		virtual void displayMenu() override;
		virtual void startChat() override;

	protected:
		bool displayChat();
		void displayLoginMenu();
		void displaySignupMenu();
		void displayLoginMenuForClient();
		void displaySignupMenuForClient();
		void displayUsersAndMessages();
		void displayMenuForClient();
		bool isCorrectPassword(const std::string& password, const std::string& login);

	protected:

};
