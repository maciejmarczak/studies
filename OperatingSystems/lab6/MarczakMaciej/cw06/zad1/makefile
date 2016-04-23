CC=gcc
CFLAGS=-o $@ -Wall

all:
	make run-server
	sleep 2
	make run-clients

client:
	$(CC) $(CFLAGS) client.c common_functions.c

server:
	$(CC) $(CFLAGS) server.c common_functions.c

clean:
	rm -f server client

run-server: server
	xterm -e ./server . t &

run-clients: client
	xterm -e ./client . t &
	sleep 3
	xterm -e ./client . t &
	sleep 3
	xterm -e ./client . t &