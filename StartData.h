#pragma once
#include <string>

struct StartData {
	//StartData() = default;
	const  char* SQL_name = "testdb";
	const  char* SQL_user = "root";
	const  char* SQL_password = "root";
	const  std::string SQL_commands = "createTables.txt";

	const  std::string users_file = "users.txt";
	const  std::string messages_file = "messages.txt";
	const  std::string log_file = "log.txt";
	std::string ip_adr = "127.0.0.1";
};