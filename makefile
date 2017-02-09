CC = g++
CFLAGS = -g -O -Wall
BIN = main.o commands.o utils.o fs.o

all: main

commands.o: commands.cpp commands.h
	$(CC) $(CFLAGS) -c commands.cpp

utils.o: utils.cpp utils.h
	$(CC) $(CFLAGS) -c utils.cpp 

fs.o: fs.cpp fs.h
	$(CC) $(CFLAGS) -c fs.cpp

main.o: main.cpp commands.cpp utils.cpp fs.cpp 
	$(CC) $(CFLAGS) -c main.cpp

main: main.o commands.o utils.o fs.o  
	$(CC) $(CFLAGS) -o $@ main.o  

clean:
	rm -f $(BIN) 
