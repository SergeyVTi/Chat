cmake_minimum_required(VERSION 3.23.2)

project(Chat)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
# -Wall -Wextra -Wpedantic -g3

set (INCS "/usr/include/mysql")
#set (LIB "/lib/libmariadb.dll.a")
find_library(MYSQL_LIBRARY mariadb)

set(SOURCES main.cpp
	Chat.cpp
    AuthData.cpp
    Message.cpp
    Connection.cpp
    Server.cpp
    Client.cpp
    DefaultHandler.cpp
    SQLdataBase.cpp) 
    
#set(HEADERS Errors.h InOut.h OS_info.h)     

#add_library(mylib ${SOURCES})
include_directories(${INCS})

add_executable(chat.out ${SOURCES})

target_link_libraries(chat.out ${MYSQL_LIBRARY})

