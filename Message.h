#pragma once

#include <string>

using namespace std;

class Message {
	public:
		explicit Message(const string& from, const string& to 
		              , const string& text = "");
		~Message() = default;
		const string& getFrom() const {return from_;}
		const string& getTo() const {return to_;}
		const string& getText() const {return text_;}


	protected:
		string from_; 
		string to_;
		string text_;
};
