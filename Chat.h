#pragma once

#include "AuthData.h"
#include "Message.h"

#include <string>
#include <exception>
#include <unordered_map>
#include <vector>
#include <iostream>

using namespace std;

class Chat {
	public:
		Chat();
		~Chat() = default;

		bool displayMenu();

		template <typename T>
		void insertUsers(T pair);
		template <typename T, typename... Args>
		void insertUsers(T pair, Args&&... users);

		template <typename T>
		void insertMessages(T message);
		template <typename T, typename... Args>
		void insertMessages(T message, Args&&... users);

	protected:

		bool displayChat(const pair<string,AuthData>& user);
		void displayLoginMenu();
		void displaySignupMenu();
		void sentMessageToAll(const pair<string,AuthData>& from);
		void sentMessageToUser(const pair<string,AuthData>&  from);
		void displayUsers();
		void displayMessages(const pair<string,AuthData>& to);
		string getString(const string& str);


		unordered_map<string,AuthData> users_; //
		vector<Message> messages_;
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

