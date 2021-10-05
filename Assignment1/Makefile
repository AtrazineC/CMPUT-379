CC		:= g++
CFLAGS	:= -Wall -O2
TARGET = shell379

all: link

link: compile
	$(CC) $(CFLAGS) -o $(TARGET) main.o command.o process_handler.o process.o signal_handler.o util.o

compile: main.o command.o process_handler.o process.o signal_handler.o util.o

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

command.o: command.cpp command.h
	$(CC) $(CFLAGS) -c command.cpp

process_handler.o: process_handler.cpp process_handler.h
	$(CC) $(CFLAGS) -c process_handler.cpp

process.o: process.cpp process.h
	$(CC) $(CFLAGS) -c process.cpp

signal_handler.o: signal_handler.cpp signal_handler.h
	$(CC) $(CFLAGS) -c signal_handler.cpp

util.o: util.cpp util.h
	$(CC) $(CFLAGS) -c util.cpp

compress:
	zip shell379.zip Makefile -r *.cpp *.h

clean:
	rm -f *.o shell379
