CC=gcc
CFLAGS=-o $@ -Wall

all:
	make run-server
	sleep 2
	make run-clients

client:
	$(CC) $(CFLAGS) client.c common_functions.c -lrt

server:
	$(CC) $(CFLAGS) server.c common_functions.c -lrt

clean:
	rm -f server client

run-server: server
	xterm -e ./server /server &

run-clients: client
	xterm -e ./client /server &
	sleep 3
	xterm -e ./client /server &
	sleep 3
	xterm -e ./client /server &