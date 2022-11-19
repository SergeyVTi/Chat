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
		Client(const StartData& data) : Chat(data) {}
		virtual ~Client() = default;

		virtual void displayMenu() override;
		virtual void startChat() override;
		virtual void setDataBase() override;
		virtual	void makeConnection() override;

	protected:

};
