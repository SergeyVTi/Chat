#pragma once

#include "AuthData.h"
#include "Message.h"

#include <string>
#include <exception>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstring>
#include <memory>

namespace fs = std::filesystem;

class Chat {
	public:
		Chat(const std::string& users_file, const std::string& messages_file);
		virtual ~Chat() = default;

		virtual bool displayMenu() = 0;

		template <typename T>
		void insertUsers(T pair);
		template <typename T, typename... Args>
		void insertUsers(T pair, Args&&... users);

		template <typename T>
		void insertMessages(T message);
		template <typename T, typename... Args>
		void insertMessages(T message, Args&&... users);

		void readUsersFromFile();
		void readMessagesFromFile();

	protected:
		
		string getString(const std::string& str);		
		void writeUserInFile(const std::string& login, const std::string& password);
		void writeMessageInFile(const Message& message);
		void show_perms(fs::perms p);

	protected:
		unordered_map<std::string,AuthData> users_; 
		vector<Message> messages_;
		const std::string users_file_;
		const std::string messages_file_;		
};

template <typename T>
void Chat::insertUsers(T pair) {
	users_.insert(pair);

	cout << "Added user " << pair.first << ", hash: ";

	uint* hash = pair.second.getPasswordHash();
	for (size_t i = 0; i < 5; ++i)
		cout << hex << hash[i] << " ";
	cout << endl;
}

template <typename T, typename... Args>
void Chat::insertUsers(T pair, Args&&... users) {
	users_.insert(pair);

	cout << "Added user " << pair.first << ", hash: ";

	uint* hash = pair.second.getPasswordHash();
	for (size_t i = 0; i < 5; ++i)
		cout << hex << hash[i] << " ";
	cout << endl;

	insertUsers(forward<Args>(users)...);
}

template <typename T>
void Chat::insertMessages(T message) {
	messages_.push_back(message);
}

template <typename T, typename... Args>
void Chat::insertMessages(T message, Args&&... users) {
	messages_.push_back(message);
	insertMessages(forward<Args>(users)...);
}

class Error : public exception {
		const char* error_;
	public:
		Error(const char* error) : error_(error) {
		}
		const char* what() const noexcept override {
			return error_;
		}
};

