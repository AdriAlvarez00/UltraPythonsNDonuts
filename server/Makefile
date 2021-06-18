CC=g++
CFLAGS=-g -I.
DEPS = Socket.h SnakeGames.h json.hpp
OBJ = Socket.o SnakeGames.o
LIBS= -lpthread

%.o: %.cc $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

all: PnDTest PnDSender

PnDTest: $(OBJ) PnDTest.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

PnDSender: $(OBJ) PnDSender.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

loadSnake: $(OBJ) loadSnake.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)


.PHONY: clean

clean:
	rm -f *.o PnDServer PnDTest PnDSender
