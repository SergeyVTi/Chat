#pragma once

#include <string>
#include <iostream>
#include <cstring>
#include <limits>

class Input {
	public:
		std::size_t getInputSelection() {
			std::size_t selection {};
			while (std::cout << "---> " && !(std::cin >> selection)) {
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Input error" << std::endl;
			}
			return selection;
		}

		std::string getInputString(const std::string& str) {
			std::string str_;
			std::cout << "Enter " << str << std::endl
			          << "---> ";
			std::cin >> str_;
			return str_;
		}
	protected:
	
};

class Output {
	public:
		std::string getSelectModeMenu() {
			std::string message;
			message += "\nSelect mode:\n";
			message += "1) Server 2) Client 3) Exit\n";
			message += "---------------------------------\n";
			return message;
		}
		std::string getLoginMenu() {
			std::string message;
			message += "\n---------------------------------\n";
			message += "1) Login 2) Registration 3) Exit\n";
			message += "---------------------------------\n";
			return message;
		}
	protected:
};