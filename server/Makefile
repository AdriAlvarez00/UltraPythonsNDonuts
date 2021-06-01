CC=g++
CFLAGS=-g -I.
DEPS = Socket.h snake.pb.h SnakeGames.h
OBJ = Socket.o PnDServer.o snake.pb.o SnakeGames.o
LIBS= -lpthread -lprotobuf

%.o: %.cc $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

all: PnDSever

PnDSever: $(OBJ) PnDServer.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)



.PHONY: clean

clean:
	rm -f *.o PnDs

