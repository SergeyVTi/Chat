
#include "Chat.h"

#include <iostream>
#include <algorithm>
using namespace std;
constexpr size_t SHA1HASHLENGTHBYTES = 20;

void Chat::makeConnection() {
	if (connection_->makeConnection() == 1)
		throw Error("ERROR: connection fail");
}

string Chat::reciveMessage() {
	return connection_->reciveMessage();
}

void Chat::sendMessage(const string& message) {
	connection_->sendMessage(message);
}

void Chat::writeUserInFile(const std::string& login,
                           const std::string& password) {
	containerHandler_->writeUserInFile(login, password);
}

void Chat::writeMessageInFile(const Message& message) {
	containerHandler_->writeMessageInFile(message);
}

unordered_map<std::string, AuthData>::iterator Chat::findUser(
    const std::string& login) {
	return containerHandler_->findUser(login);
}

bool Chat::isUserExists(const string& login) {
	return containerHandler_->isUserExists(login);
}

void Chat::setUserName(const string& login) {
	containerHandler_->setUserName(login);
}

void Chat::setServerName(const string& login) {
	containerHandler_->setServerName(login);
}

string Chat::getServerName() {
	return containerHandler_->getServerName();
}

string Chat::getUserName() {
	return containerHandler_->getUserName();
}

void Chat::addUsers(pair<string, AuthData> pair) {
	containerHandler_->addUsers(pair);
}

void Chat::addMessage(const string& from, const string& to
                      , const string& text) {
	containerHandler_->addMessage(from, to, text);
}
