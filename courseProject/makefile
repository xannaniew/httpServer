CC=g++
EXCEPTIONS_DIR=exceptions/

all: Socket_error_exception.o Server.o main.o
	$(CC) $(EXCEPTIONS_DIR)Socket_error_exception.o Server.o main.o -o main

Socket_error_exception.o: $(EXCEPTIONS_DIR)Socket_error_exception.cpp
	$(CC) -c $(EXCEPTIONS_DIR)Socket_error_exception.cpp -o $(EXCEPTIONS_DIR)Socket_error_exception.o

Server.o: Server.cpp
	$(CC) -c Server.cpp -o Server.o

main.o: main.cpp 
	$(CC) -c main.cpp -o main.o