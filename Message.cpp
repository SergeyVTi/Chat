#include "Message.h"
#include <iostream>

using namespace std;

Message::Message(const string& from, const string& to
                 , const string& text)
	: from_(from)
	, to_(to)
	, text_(text) {

}

