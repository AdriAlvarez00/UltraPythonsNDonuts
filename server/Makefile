CC=g++
CFLAGS=-g -I.
DEPS = Socket.h snake.pb.h SnakeGames.h
OBJ = Socket.o snake.pb.o SnakeGames.o 
LIBS= -lpthread -lprotobuf

%.o: %.cc $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

all: PnDTest loadSnake PnDSender

PnDTest: $(OBJ) PnDTest.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

PnDSender: $(OBJ) PnDSender.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

loadSnake: $(OBJ) loadSnake.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)


.PHONY: clean

clean:
	rm -f *.o PnDServer PnDTest PnDSender
