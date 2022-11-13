#pragma once

//#include "AuthData.h"//
//#include "Message.h"//
#include "Connection.h"
#include "FileIO.h"

#include <string>
//#include <exception>
#include <unordered_map>
//#include <vector>//
#include <iostream>
//#include <fstream>//
#include <filesystem>
//#include <cstring>//
#include <memory>

namespace fs = std::filesystem;//

class Chat {
	public:
		Chat(const std::string& users_file, const std::string& messages_file);
		virtual ~Chat() = default;

		virtual void displayMenu() = 0;
		void makeConnection();
		virtual void startChat() = 0;

		void readUsersFromFile();
		void readMessagesFromFile();

	protected:

		std::string getString(const std::string& str);

		void writeUserInFile(const std::string& login,
		                     const std::string& password);
		void writeMessageInFile(const Message& message);
		std::unordered_map<std::string, AuthData>::iterator findUser(
		    const std::string& login);
		bool isUserExists(const std::string& login);
		void setUserName(const std::string& login);
		void setServerName(const std::string& login);
		std::string getServerName();
		std::string getUserName();
		std::string reciveMessage();
		void sendMessage(const std::string& message);
		void addUsers(std::pair<std::string, AuthData> pair);
		void addMessage(const std::string& from, const std::string& to,
		                const std::string& text);

	protected:
		bool successfulAccessToAccount_ = 0;
		std::unique_ptr<Connection> connection_ = nullptr;
		std::unique_ptr<FileIO> containerHandler_ = nullptr;
};


