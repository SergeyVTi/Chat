#pragma once

#include <string>
#include <iostream>
#include <cstring>
#include <limits>

class Input {
	public:
		std::size_t getInputSelection() {
			while (std::cout << "--->" && !(std::cin >> selection_)) {
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Input error" << std::endl;
			}
			return selection_;
		}
	protected:
		std::size_t selection_ {};
};

class Output {
	public:
		std::string getSelectModeMenu() {
			message_ += "\nSelect mode:\n";
			message_ += "1) Server 2) Client 3) Exit\n";
			message_ += "---------------------------------\n";
			return message_;
		}
		std::string getLoginMenu() {
			message_ += "\n---------------------------------\n";
			message_ += "1) Login 2) Registration 3) Exit\n";
			message_ +=	"---------------------------------\n";
			return message_;
		}
	protected:
		std::string message_;
};