CC = g++
CFLAGS = -g -O -Wall 
BIN = main.o commands.o utils.o fs.o

#all: commands.o utils.o fs.o
#	$(CC) $(CFLAGS) commands.o utils.o fs.o -o all

main.o: main.cpp commands.h utils.h fs.h
	$(CC) $(CFLAGS) -c main.cpp

commands.o: commands.cpp commands.h
	$(CC) $(CFLAGS) -c commands.cpp

utils.o: utils.cpp utils.h
	$(CC) $(CFLAGS) -c utils.cpp 

fs.o: fs.cpp fs.h
	$(CC) $(CFLAGS) -c fs.cpp

clean:
	rm -f $(BIN) 
