SRC=BufferManager
OBJS = BufferManager.o BufferFrame.o

CFLAGS = -Wall -c -g -std=c++11
LFLAGS = -Wall -g -std=c++11
CC = g++

all: $(OBJS) main.o
	$(CC) $(LFLAGS) $(OBJS) main.o
    
clean:
	rm -rf ./*.o
	rm a.out

main.o:
	$(CC) $(CFLAGS) $(SRC)/main.cpp
        
BufferManager.o:
	$(CC) $(CFLAGS) $(SRC)/BufferManager.cpp

BufferFrame.o:
	$(CC) $(CFLAGS) $(SRC)/BufferFrame.cpp
