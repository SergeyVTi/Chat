#pragma once

#include <string>

class Message {
	public:
		explicit Message(const std::string& from, const std::string& to 
		              , const std::string& text = "");
		~Message() = default;
		const std::string& getFrom() const {return from_;}
		const std::string& getTo() const {return to_;}
		const std::string& getText() const {return text_;}


	protected:
		std::string from_; 
		std::string to_;
		std::string text_;
};
