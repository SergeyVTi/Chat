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
		Client(const string& users_file, const string& messages_file,
		       const string& ip_file);
		virtual ~Client() = default;

		virtual bool displayMenu() override;

	protected:

	protected:
		unique_ptr<Connection> Connection_;

};
