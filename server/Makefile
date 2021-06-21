CC=g++
CFLAGS=-g -I.
DEPS = Socket.h SnakeGames.h json.hpp SnakeServer.h SnakeMessages.h
OBJ = Socket.o SnakeGames.o SnakeServer.o
LIBS= -lpthread

%.o: %.cc $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

all: release tests

release: PnDSender PnDServer

tests: PnDTest PnDDOS PnDumbClient

PnDTest: $(OBJ) PnDTest.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

PnDSender: $(OBJ) PnDSender.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

PnDServer: $(OBJ) PnDServer.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

PnDDOS: $(OBJ) PnDDOS.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

PnDumbClient: $(OBJ) PnDumbClient.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o PnDServer PnDTest PnDSender loadSnake PnDDOS
