CC=g++
CFLAGS=-g -I.
DEPS = Socket.h snake.pb.h SnakeGames.h SnakeServer.h SnakeClient.h
OBJ = Socket.o snake.pb.o SnakeGames.o SnakeServer.o SnakeClient.o
LIBS= -lpthread -lprotobuf

%.o: %.cc $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

all: PnDTest PnDSender PnDServer PnDClient

PnDTest: $(OBJ) PnDTest.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

PnDSender: $(OBJ) PnDSender.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

PnDClient: $(OBJ) PnDClient.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

PnDServer: $(OBJ) PnDServer.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

loadSnake: $(OBJ) loadSnake.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)


.PHONY: clean

clean:
	rm -f *.o PnDServer PnDTest PnDSender
