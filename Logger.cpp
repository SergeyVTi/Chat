#include "Logger.h"
#include "StartData.h"

using namespace std;

Logger::Logger() {

}

Logger::~Logger() {
	log_file_stream_.close();
}

void Logger::init() {
	StartData startData;
	log_file_stream_ = fstream(startData.log_file,
	                           ios::out | ios::trunc);

	start_ = chrono::system_clock::now();
}

fstream& Logger::operator<<(const std::string& message) {
	call_once(initialized_, &Logger::init, this);

	lock_guard<mutex> raii(mutex_);

	auto end = chrono::system_clock::now();
	chrono::duration<double> diff = end - start_;

	log_file_stream_ << fixed << setprecision(4) << left;

	log_file_stream_ << "[" << diff.count() << "] " << message;
	
	return log_file_stream_;
}