all: rdfifo wrfifo
	xterm -e ./rdfifo test &
	sleep 0.5
	xterm -e ./wrfifo test &

rdfifo:
	gcc -Wall -o rdfifo rdfifo.c

wrfifo:
	gcc -Wall -o wrfifo wrfifo.c

clean:
	rm wrfifo rdfifo