
#include "Chat.h"

#include <iostream>
#include <algorithm>

using namespace std;
constexpr size_t SHA1HASHLENGTHBYTES = 20;

string Chat::reciveMessage() {
	return connection_->reciveMessage();
}

void Chat::sendMessage(const string& message) {
	connection_->sendMessage(message);
}

void Chat::writeUserInDataBase(const std::string& login,
                               const std::string& password) {
	containerHandler_->writeUserInDataBase(login, password);
}

void Chat::writeMessageInDataBase(const Message& message) {
	containerHandler_->writeMessageInDataBase(message);
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
