#pragma once
#include <exception>

class Error : public std::exception {
		const char* error_;
	public:
		Error(const char* error) : error_(error) {
		}
		const char* what() const noexcept override {
			return error_;
		}
};

class Exit : public std::exception {
		const char* error_;
	public:
		Exit() : error_("Exit") {
		}
		const char* what() const noexcept override {
			return error_;
		}
};