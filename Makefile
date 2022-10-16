SRC = main.cpp
TARGET = chat.out
LIB = ChatLib
PREF = /usr/local/bin
FLAGS = g++ -std=c++17 -Wall -Wextra -Wpedantic

chat : $(SRC) lib
	g++ -std=c++17 -Wall -Wextra -Wpedantic -o $(TARGET) $(SRC) -L. -l$(LIB)
	
lib :  AuthData.cpp AuthData.h Chat.cpp Chat.h Message.cpp Message.h Connection.cpp Connection.h Server.cpp Server.o Client.cpp Client.o
	$(FLAGS) -o AuthData.o AuthData.cpp -c
	$(FLAGS) -o Chat.o Chat.cpp -c
	$(FLAGS) -o Message.o Message.cpp -c
	$(FLAGS) -o Connection.o Connection.cpp -c
	$(FLAGS) -o Server.o Server.cpp -c
	$(FLAGS) -o Client.o Client.cpp -c
	ar rc lib$(LIB).a AuthData.o Chat.o Message.o Connection.o Server.o Client.o
	export LD_LIBRARY_PATH=.

clean : 
	rm -rf *.o *.a

install :
	sudo install -v $(TARGET) $(PREF)

uninstall :
	sudo rm -rfv $(PREF)/$(TARGET)
