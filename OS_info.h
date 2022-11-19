#pragma once

#include <iostream>

#if defined(_WIN64)
#define PLATFORM_NAME "Windows 64-bit" // Windows 64-bit
#elif defined(__linux__) // Linux
#define PLATFORM_NAME "Linux"
#define LINUX
#include <sys/utsname.h>
#else
#define LINUX
#define PLATFORM_NAME "OS not detected"
#endif

class OS {
	public:
		void displayVersion() {
#if defined (__linux__)
			struct utsname utsname;
			uname(&utsname);

			std::cout << "OS name: " << utsname.sysname << " " << utsname.machine <<
			     std::endl;
			std::cout << "OS version: " << utsname.version << std::endl;
			std::cout << "OS version: "<< PLATFORM_NAME << std::endl;
#else
			std::cout << "OS version: "<< PLATFORM_NAME << std::endl;
#endif
		}
};

