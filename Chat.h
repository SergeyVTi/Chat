#pragma once

#include "Connection.h"
#include "SQLdataBase.h"
#include "InOut.h"
#include "Logger.h"
#include "StartData.h"

#include <string>
#include <unordered_map>
#include <iostream>
//#include <filesystem>
//#include <memory>

//namespace fs = std::filesystem;//

class Chat {
	public:
		Chat() = default; //const StartData& data startData_(data) 
		
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
		std::unique_ptr<SQLdataBase> containerHandler_ = nullptr;
		Input input_;
		Output output_;
		StartData startData_;
};




