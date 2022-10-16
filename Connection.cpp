#include "Connection.h"
#include <iostream>

using namespace std;



#if defined LINUX
ServerLin64TCPIP::~ServerLin64TCPIP() {
	close(socket_file_descriptor);
}

int ServerLin64TCPIP::makeConnection() {
	// Создадим сокет
	socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_file_descriptor == -1) {
		cout << "Socket creation failed.!" << endl;
		exit(1);
	}
	//
	serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);
	// Зададим номер порта для связи
	serveraddress.sin_port = htons(DEFAULT_PORT1);
	// Используем IPv4
	serveraddress.sin_family = AF_INET;
	// Привяжем сокет
	bind_status = bind(socket_file_descriptor,
	                   (struct sockaddr*)&serveraddress,
	                   sizeof(serveraddress));
	if(bind_status == -1)  {
		cout << "Socket binding failed.!" << endl;
		exit(1);
	}
	// Поставим сервер на прием данных
	connection_status = listen(socket_file_descriptor, 5);
	if(connection_status == -1) {
		cout << "Socket is unable to listen for new connections.!" << endl;
		exit(1);
	}  else  {
		cout << "Server is listening for new connection: " << endl;
	}
	length = sizeof(client);
	connection = accept(socket_file_descriptor,(struct sockaddr*)&client,
	                    &length);
	if(connection == -1)  {
		cout << "Server is unable to accept the data from client.!" << endl;
		exit(1);
	}

	cout << "Client connected!" << endl;
	// Communication Establishment
	return 0;
}

int ServerLin64TCPIP::sendMessage(const string& mes) {
	write(connection, mes.c_str(), mes.length());

	return 0;
}

string ServerLin64TCPIP::reciveMessage() {
	memset(message,0,sizeof(message));
	read(connection, message, sizeof(message));

	string mes;
	mes = message;

	return mes;
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
ClientLin64TCPIP::ClientLin64TCPIP(const string& ip_file) : ip_adr_
	(ip_file) {

}

ClientLin64TCPIP::~ClientLin64TCPIP() {
	close(socket_file_descriptor);
}

int ClientLin64TCPIP::makeConnection() {
// Создадим сокет
	socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_file_descriptor == -1) {
		cout << "Creation of Socket failed!" << endl;
		exit(1);
	}

	// Установим адрес сервера
	serveraddress.sin_addr.s_addr = inet_addr(ip_adr_.c_str());
	// Зададим номер порта
	serveraddress.sin_port = htons(DEFAULT_PORT1);
	// Используем IPv4
	serveraddress.sin_family = AF_INET;
	// Установим соединение с сервером
	connection = connect(socket_file_descriptor,
	                     (struct sockaddr*)&serveraddress, sizeof(serveraddress));
	if(connection == -1) {
		cout << "Connection with the server failed.!" << endl;
		exit(1);
	}
	// Взаимодействие с сервером
	cout << "Connected to server!" << endl;
	return 0;
}

int ClientLin64TCPIP::sendMessage(const string& mes) {
	write(socket_file_descriptor, mes.c_str(), mes.length());

	return 0;
}

string ClientLin64TCPIP::reciveMessage() {
	memset(message,0,sizeof(message));
	read(socket_file_descriptor, message, sizeof(message));
	string mes;
	mes = message;
	return mes;
}
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

#elif defined(_WIN64)

ServerWin64TCPIP::~ServerWin64TCPIP() {
	closesocket(ClientSocket);
	WSACleanup();
}

int ServerWin64TCPIP::makeConnection() {
	iSendResult = 0;
	memset(recvbuf, 0, sizeof(recvbuf));

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT2, &hints, &result);
	if ( iResult != 0 ) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Create a SOCKET for the server to listen for client connections.
	ListenSocket = socket(result->ai_family, result->ai_socktype,
	                      result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// Setup the TCP listening socket
	iResult = bind( ListenSocket, result->ai_addr,
	                (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	cout << "Waiting for client..." << endl;

	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	cout << "Client connected" << endl;

	// No longer need server socket ??
	closesocket(ListenSocket);

	return 0;
}

int ServerWin64TCPIP::sendMessage(const string& message) {

	iSendResult = send( ClientSocket, message.c_str(), message.length(), 0);

	if (iSendResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		//closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}
	return 0;
}

string ServerWin64TCPIP::reciveMessage() {
	string message;
	memset(recvbuf,0,sizeof(recvbuf));

	iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
	if (iResult <= 0) {
		printf("recv failed with error: %d\n", WSAGetLastError());
		//closesocket(ClientSocket);
		WSACleanup();
		return "1";
	}

	message = recvbuf;

	return message;
}

ClientWin64TCPIP::~ClientWin64TCPIP() {
	closesocket(ConnectSocket);
	WSACleanup();
}

int ClientWin64TCPIP::makeConnection() {

	const char *sendbuf = "this is a test";

	// Validate the parameters
	if (ip_adr_.size() != 0) {
		cout << "Using ip adress: " << ip_adr_ << endl;
	}

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory( &hints, sizeof(hints) );
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(ip_adr_.c_str(), DEFAULT_PORT2, &hints, &result);
	if ( iResult != 0 ) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Attempt to connect to an address until one succeeds
	for(auto ptr=result; ptr != NULL ; ptr=ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
		                       ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		// Connect to server.
		iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}

	cout << "Connected to server!" << endl;
	return 0;
}

int ClientWin64TCPIP::sendMessage(const std::string& message) {

	iResult = send( ConnectSocket, message.c_str(), message.length(), 0 );
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	return 0;
}

string ClientWin64TCPIP::reciveMessage() {
	memset(recvbuf,0,sizeof(recvbuf));

	iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);	
	
	string message;
	message = recvbuf;

	return message;
}

#endif