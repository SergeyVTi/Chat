#pragma once

#include "DataBase.h"
#include "Chat.h"
#include <mysql.h>

class SQLdataBase : public DataBase {
	public:
		SQLdataBase(const StartData& data) : DataBase(), startData_(data) {}

		virtual void readUsersFromDataBase() override;
		virtual void readMessagesFromDataBase() override;
		virtual void writeMessageInFile(const Message& message) override;
		virtual void writeUserInFile(const std::string& login,
		                             const std::string& password) override;
		virtual std::unordered_map<std::string, AuthData>::iterator findUser(
		    const std::string& login) override;
		virtual bool isUserExists(const std::string& login) override;
		virtual void addMessage(const std::string& from,
		                        const std::string& to, const std::string& text) override;
		virtual std::string displayUsersAndMessages() override;
		virtual	void setUserName(const std::string& login) override;
		virtual void setServerName(const std::string& login) override;
		virtual std::string getServerName() override;
		virtual std::string getUserName() override;
		virtual bool isConnectedToSQLdataBase() override;

	protected:
		void createTables();
		void printResult();
			 
	protected:
		MYSQL mysql_;
		MYSQL_RES* result_;
		MYSQL_ROW row_;
		StartData startData_;
};


class DefaultHandler : public DataBase {
	public:
		DefaultHandler(const std::string& users_file,
		               const std::string& messages_file) : DataBase(users_file,
			                       messages_file) {
		}

		void showPermissions(std::filesystem::perms p);
		virtual void readUsersFromDataBase() override;
		virtual void readMessagesFromDataBase() override;
		virtual void writeMessageInFile(const Message& message) override;
		virtual void writeUserInFile(const std::string& login,
		                             const std::string& password) override;
		virtual std::unordered_map<std::string, AuthData>::iterator findUser(
		    const std::string& login) override;
		virtual bool isUserExists(const std::string& login) override;
		void addMessage(const std::string& from, const std::string& to,
		                const std::string& text);
		virtual std::string displayUsersAndMessages() override;
		virtual void setUserName(const std::string& login) override;
		virtual void setServerName(const std::string& login) override;
		virtual std::string getServerName() override;
		virtual std::string getUserName() override;
		virtual bool isConnectedToSQLdataBase() override;

	protected:
};