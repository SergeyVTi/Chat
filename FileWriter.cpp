#include "FileIO.h"

#include <iostream>

using namespace std;
namespace fs = std::filesystem;

void FileWriter::writeMessageInFile(const Message& message) {
	fstream messages_file_stream = fstream(messages_file_,
	                                       ios::in | ios::out | ios::ate);
	if (!messages_file_stream) {
		messages_file_stream = fstream(messages_file_,
		                               ios::in | ios::out | ios::trunc);
	}

	if (messages_file_stream) {
		messages_file_stream << endl
		                     << message.getFrom() << " "
		                     << message.getTo() << " "
		                     << message.getText();
	}
	messages_file_stream.close();
}

void FileWriter::writeUserInFile(const string& login,
                           const string& password) {
	fstream user_file_stream = fstream(users_file_,
	                                   ios::in | ios::out | ios::ate);
	if (!user_file_stream) {
		user_file_stream = fstream(users_file_,
		                           ios::in | ios::out | ios::trunc);
	}

	if (user_file_stream) {
		user_file_stream << endl << login << " " << password;
	}
	user_file_stream.close();
}