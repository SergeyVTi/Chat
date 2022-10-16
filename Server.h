#pragma once
#include "Chat.h"
#include "Connection.h"

#if defined(__linux__)
#define LINUX
#else
#define LINUX 
#endif

class Server : public Chat {
	public:
		Server(const string& users_file, const string& messages_file);
		virtual ~Server() = default;

		virtual bool displayMenu() override;


	protected:
		bool displayChat();
		void displayLoginMenu();
		void displaySignupMenu();
		void displayLoginMenuForClient();
		void displaySignupMenuForClient();
		void displayUsersAndMessages();

	protected:
		unique_ptr<Connection> Connection_;
		string server_name_;
		string user_name_;
};
