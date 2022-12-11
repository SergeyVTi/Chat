#pragma once

#include <mutex>
#include <fstream>
#include <chrono>
#include <iomanip>

class Logger {
	public:
		Logger();
		~Logger();
		std::fstream& operator<<(const std::string& message);

	protected:
		void init();

	protected:		
		std::mutex mutex_;
		std::once_flag initialized_;
		std::fstream log_file_stream_;
		std::chrono::time_point<std::chrono::system_clock> start_;
};