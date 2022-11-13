#pragma once
#include <string>

#if defined(_WIN64)
#undef UNICODE
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
//#pragma comment (lib, "Mswsock.lib")
#elif defined(__linux__)
#define LINUX
#else
#define LINUX
#endif


#if defined LINUX
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

constexpr size_t DEFAULT_BUFLEN = 1024;
constexpr size_t DEFAULT_PORT1 = 27015;
constexpr char* DEFAULT_PORT2  = "27015";

class Connection {
	public:
		Connection() = default;
		virtual ~Connection() = default;
		virtual int makeConnection() = 0;
		virtual int sendMessage(const std::string& message) = 0;
		virtual std::string reciveMessage() = 0;
	protected:
};

#if defined LINUX

class ServerLinuxTCP : public Connection {
	public:
		ServerLinuxTCP() = default;
		virtual ~ServerLinuxTCP();

	protected:
		virtual int makeConnection() override;
		virtual int sendMessage(const std::string& message) override;
		virtual std::string reciveMessage() override;

	protected:
		struct sockaddr_in serveraddress, client;
		socklen_t length;
		int socket_file_descriptor, connection, bind_status,
		    status;
		char message[DEFAULT_BUFLEN];
};

class ClientLinuxTCP : public ServerLinuxTCP {
	public:
		ClientLinuxTCP(const std::string& ip_file);
		virtual ~ClientLinuxTCP();

	protected:
		virtual int makeConnection() override;
		virtual int sendMessage(const std::string& message) override;
		virtual std::string reciveMessage() override;

	protected:
		std::string ip_adr_;
};

#elif defined(_WIN64)

class ServerWinTCP : public Connection {
	public:
		ServerWinTCP() = default;
		virtual ~ServerWinTCP();

	protected:
		virtual int makeConnection() override;
		virtual int sendMessage(const std::string& message) override;
		virtual std::string reciveMessage() override;

	protected:
		WSADATA wsaData;
		int iResult;

		SOCKET ListenSocket = INVALID_SOCKET;
		SOCKET ClientSocket = INVALID_SOCKET;
		SOCKET ConnectSocket = INVALID_SOCKET;

		struct addrinfo *result = NULL;
		struct addrinfo hints;

		int iSendResult;
		char recvbuf[DEFAULT_BUFLEN];
		int recvbuflen = DEFAULT_BUFLEN;
};

class ClientWinTCP : public ServerWinTCP {
	public:
		ClientWinTCP(const std::string& ip_file) : ip_adr_(ip_file){
		}
		virtual ~ClientWinTCP();

	protected:
		virtual int makeConnection() override;
		virtual int sendMessage(const std::string& message) override;
		virtual std::string reciveMessage() override;
	protected:
		std::string ip_adr_;
};
#endif
