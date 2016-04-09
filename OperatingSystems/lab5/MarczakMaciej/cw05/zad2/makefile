all: rdfifo wrfifo
	./rdfifo test
	./wrfifo test

rdfifo:
	gcc -Wall -o rdfifo rdfifo.c

wrfifo:
	gcc -Wall -o wrfifo wrfifo.c

clean:
	rm wrfifo rdfifo