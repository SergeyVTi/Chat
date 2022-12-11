#pragma once

//#include "DataBase.h"
#include "AuthData.h"
#include "Message.h"
#include "Errors.h"
//#include "Chat.h"
#include "Logger.h"
#include "StartData.h"

#include <mysql.h>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <filesystem>

class SQLdataBase {
	public:
		SQLdataBase() = default;

		void readUsersFromDataBase();
		void readMessagesFromDataBase();
		void writeMessageInDataBase(const Message& message);
		void writeUserInDataBase(const std::string& login,
		                         const std::string& password);
		std::unordered_map<std::string, AuthData>::iterator findUser(
		    const std::string& login);
		bool isUserExists(const std::string& login);
		void addMessage(const std::string& from,
		                const std::string& to, const std::string& text);
		std::string displayUsersAndMessages();
		void setUserName(const std::string& login);
		void setServerName(const std::string& login);
		std::string getServerName();
		std::string getUserName();
		bool isConnectedToSQLdataBase();
		void setLogger(std::shared_ptr<Logger> logger);

		template <typename T>
		void addUsers(T pair);
		template <typename T, typename... Args>
		void addUsers(T pair, Args&&... users);

		template <typename T>
		void insertMessages(T message);
		template <typename T, typename... Args>
		void insertMessages(T message, Args&&... users);

	protected:
		void createTables();
		void printResult();
		void addUsersToSQLFromFile();
		std::vector<std::string> getResultsFromSQL();
		std::string showPermissions(std::filesystem::perms p);

	protected:
		MYSQL mysql_;
		MYSQL_RES* result_;
		MYSQL_ROW row_;
		StartData startData;
		std::shared_ptr<Logger> logger_ = nullptr;

		std::unordered_map<std::string, AuthData> users_;
		std::vector<Message> messages_;
		std::string user_name_;
		std::string server_name_;
};

template <typename T>
void SQLdataBase::addUsers(T pair) {
	std::string message;

	if (isUserExists(pair.first)) {
		message = "Added user "+pair.first+" exists";
		*logger_ << message << std::endl;
		return;
	}

	users_.insert(pair);
	message = "Added user "+pair.first+", hash: ";

	uint* hash = pair.second.getPasswordHash();
	for (size_t i = 0; i < 5; ++i)
		//std::cout << hash[i] << " ";//<< std::hex
		message+=std::to_string(hash[i])+" ";

	*logger_ << message << std::endl;
}

template <typename T, typename... Args>
void SQLdataBase::addUsers(T pair, Args&&... users) {
	users_.insert(pair);
	std::string message;
	message = "Added user "+pair.first+", hash: ";

	uint* hash = pair.second.getPasswordHash();
	for (size_t i = 0; i < 5; ++i)
		message+=std::to_string(hash[i])+" ";

	*logger_ << message << std::endl;

	addUsers(std::forward<Args>(users)...);
}

template <typename T>
void SQLdataBase::insertMessages(T message) {
	messages_.emplace_back(message);
}

template <typename T, typename... Args>
void SQLdataBase::insertMessages(T message, Args&&... users) {
	messages_.emplace_back(message);
	insertMessages(std::forward<Args>(users)...);
}