CC=g++
CFLAGS=-o main

all:
	$(CC) $(CFLAGS) main.cpp -lm
	./main > results.txt
	cat results.txt
	gnuplot approx_fun.plt

clean:
	rm -f main results.txt nodes.txt