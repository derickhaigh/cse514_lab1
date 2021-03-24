SHAREDIR=shared
CC=g++
CFLAGS=-I$(SHAREDIR)
OBJ = server.o client.o

all: $(OBJ)

server.o:
	$(CC) -o server.o server/P2P_server.cpp shared/P2P_shared.cpp
client.o:
	$(CC) -o client.o client/P2P_client.cpp shared/P2P_shared.cpp

clean:
	rm -f $(OBJ)
