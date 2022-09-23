SRC = main.cpp
TARGET = chat
LIB = ChatLib
PREF = /usr/local/bin

chat : $(SRC) lib
	g++ -std=c++17 -o $(TARGET) $(SRC) -L. -l$(LIB)
	make clean

lib :  AuthData.cpp AuthData.h Chat.cpp Chat.h Message.cpp Message.h
	g++ -std=c++17 -o AuthData.o AuthData.cpp -c
	g++ -std=c++17 -o Chat.o Chat.cpp -c
	g++ -std=c++17 -o Message.o Message.cpp -c
	ar rc lib$(LIB).a AuthData.o Chat.o Message.o
	export LD_LIBRARY_PATH=.

clean : 
	rm -rf *.o *.a

install :
	sudo install -v $(TARGET) $(PREF)

uninstall :
	sudo rm -rfv $(PREF)/$(TARGET)
