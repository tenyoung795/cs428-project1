CC=gcc
all: server.o client.o

server.o:
		$(CC) -o server.o -std=c11 server.c talk.c
client.o:
		$(CC) -o client.o -std=c11 client.c talk.c
clean:
	rm *.o *.out