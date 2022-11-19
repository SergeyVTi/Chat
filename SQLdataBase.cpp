#include "SQLdataBase.h"

using namespace std;

bool SQLdataBase::isConnectedToSQLdataBase() {
	mysql_init(&mysql_);
	if (&mysql_ == NULL) {
		// Если дескриптор не получен — выводим сообщение об ошибке
		cout << "Error: can't create MySQL-descriptor" << endl;
		return false;
	}
	// Подключаемся к серверу
	if (!mysql_real_connect(&mysql_, "localhost", startData_.SQL_user,
	                        startData_.SQL_password, startData_.SQL_name, NULL, NULL, 0)) {
		// Если нет возможности установить соединение с БД выводим сообщение об ошибке
		cout << "Error: can't connect to database " << mysql_error(
		         &mysql_) << endl;
		return false;
	} else {
		// Если соединение успешно установлено
		cout << "Connected to SQL database " << startData_.SQL_name << "!" <<
		     endl;
	}

	mysql_set_character_set(&mysql_, "utf8");

	createTables();
	return true;
}

void SQLdataBase::createTables() {
	fstream commands_stream = fstream(startData_.SQL_commands,
	                                  ios::in | ios::out);
	if (commands_stream)
		cout << "Creating SQL tables:" << endl;
			 
	string commands;
	while (!commands_stream.eof()) {
		getline(commands_stream, commands, ';');

		if (commands.size() > 2) {
			mysql_query(&mysql_, commands.c_str());
			printResult();
		}
	}
}

void SQLdataBase::printResult() {
	//Выводим все что есть в базе через цикл
	if (result_ = mysql_store_result(&mysql_)) {
		while (row_ = mysql_fetch_row(result_)) {
			for (int i = 0; i < mysql_num_fields(result_); i++) {
				cout << row_[i] << "  ";
			}
			cout << endl;
		}
	} else {
		cout <<  mysql_error(&mysql_) << endl;
	}
}

void SQLdataBase::readUsersFromDataBase() {

}

void SQLdataBase::readMessagesFromDataBase() {

}

void SQLdataBase::writeMessageInFile(const Message& message) {

}

void SQLdataBase::writeUserInFile(const std::string& login,
                                  const std::string& password) {

}

std::unordered_map<std::string, AuthData>::iterator
SQLdataBase::findUser(
    const std::string& login) {

}

bool SQLdataBase::isUserExists(const std::string& login) {

}

void SQLdataBase::addMessage(const std::string& from,
                             const std::string& to, const std::string& text) {

}

std::string SQLdataBase::displayUsersAndMessages() {

}

void SQLdataBase::setUserName(const std::string& login) {

}

void SQLdataBase::setServerName(const std::string& login) {

}

std::string SQLdataBase::getServerName() {

}

std::string SQLdataBase::getUserName() {

}