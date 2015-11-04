CFLAGS=-std=c11 -Wall -Wextra

all: server client

server: server.c talk.o
client: client.c talk.o
talk.o: talk.c talk.h

.PHONY: clean

clean:
	rm -f *.o server client
