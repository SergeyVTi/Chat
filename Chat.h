#pragma once

#include "Connection.h"
#include "DataBase.h"
#include "InOut.h"
//#include "OS_info.h"
//#include "Errors.h"

#include <string>
#include <unordered_map>
#include <iostream>
#include <filesystem>
#include <memory>

namespace fs = std::filesystem;//

struct StartData {
	const  char* SQL_name = "testdb";
	const  char* SQL_user = "root";
	const  char* SQL_password = "root";
	const  std::string SQL_commands = "createTables.txt";

	const  std::string users_file = "users.txt";
	const  std::string messages_file = "messages.txt";
	std::string ip_adr = "127.0.0.1";
};

class Chat {
	public:
		Chat(const StartData& data) : startData(data) {
		}
		virtual ~Chat() = default;

		virtual void displayMenu() = 0;
		virtual void makeConnection() = 0;
		virtual void startChat() = 0;
		virtual void setDataBase() = 0;

	protected:

		void writeUserInDataBase(const std::string& login,
		                     const std::string& password);
		void writeMessageInDataBase(const Message& message);
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
		std::unique_ptr<DataBase> containerHandler_ = nullptr;
		Input input;
		Output output;
		StartData startData;
};




