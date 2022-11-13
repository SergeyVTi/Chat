#pragma once
#include "Chat.h"
#include "Connection.h"

#if defined(__linux__)
#define LINUX
#else
#define LINUX
#endif

class Client : public Chat {
	public:
		Client(const std::string& users_file, const std::string& messages_file,
		       const std::string& ip_file);
		virtual ~Client() = default;

		virtual void displayMenu() override;
		virtual void startChat() override;

	protected:

};
