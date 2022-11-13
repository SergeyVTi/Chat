#pragma once

#include "AuthData.h"
#include "Message.h"
#include "Errors.h"

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <filesystem>
#include <fstream>

class FileIO {
	public:
		FileIO() = default;
		FileIO(const std::string& users_file,
		       const std::string& messages_file) : users_file_(users_file),
			messages_file_(messages_file) {
		}
		virtual ~FileIO() = default;
		virtual void readUsersFromFile() = 0;
		virtual void readMessagesFromFile() = 0;
		virtual void writeMessageInFile(const Message& message) = 0;
		virtual void writeUserInFile(const std::string& login,
		                             const std::string& password) = 0;
		virtual std::unordered_map<std::string, AuthData>::iterator findUser(
		    const std::string& login) = 0;
		virtual bool isUserExists(const std::string& login) = 0;
		virtual void addMessage(const std::string& from,
		                        const std::string& to, const std::string& text) = 0;
		virtual std::string displayUsersAndMessages() = 0;
		virtual	void setUserName(const std::string& login) = 0;
		virtual void setServerName(const std::string& login) = 0;
		virtual std::string getServerName() = 0;
		virtual std::string getUserName() = 0;

		template <typename T>
		void addUsers(T pair);
		template <typename T, typename... Args>
		void addUsers(T pair, Args&&... users);

		template <typename T>
		void insertMessages(T message);
		template <typename T, typename... Args>
		void insertMessages(T message, Args&&... users);

	protected:
		const std::string users_file_;
		const std::string messages_file_;
		std::unordered_map<std::string, AuthData> users_;
		std::vector<Message> messages_;
		std::string user_name_;
		std::string server_name_;
};

class ContainerHandler : public FileIO {
	public:
		ContainerHandler(const std::string& users_file,
		                 const std::string& messages_file) : FileIO(users_file,
			                         messages_file) {
		}

		void showPermissions(std::filesystem::perms p);
		virtual void readUsersFromFile() override;
		virtual void readMessagesFromFile() override;
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

	protected:
};


template <typename T>
void FileIO::addUsers(T pair) {
	if (isUserExists(pair.first))
		return;

	users_.insert(pair);

	std::cout << "Added user " << pair.first << ", hash: ";

	uint* hash = pair.second.getPasswordHash();
	for (size_t i = 0; i < 5; ++i)
		std::cout << std::hex << hash[i] << " ";
	std::cout << std::endl;
}

template <typename T, typename... Args>
void FileIO::addUsers(T pair, Args&&... users) {
	users_.insert(pair);

	std::cout << "Added user " << pair.first << ", hash: ";

	uint* hash = pair.second.getPasswordHash();
	for (size_t i = 0; i < 5; ++i)
		std::cout << std::hex << hash[i] << " ";
	std::cout << std::endl;

	addUsers(std::forward<Args>(users)...);
}

template <typename T>
void FileIO::insertMessages(T message) {
	messages_.emplace_back(message);
}

template <typename T, typename... Args>
void FileIO::insertMessages(T message, Args&&... users) {
	messages_.emplace_back(message);
	insertMessages(std::forward<Args>(users)...);
}
