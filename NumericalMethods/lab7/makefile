CC=g++
CFLAGS=-o main
LIBS=-lm
M=e
S=100

all:
	$(CC) $(CFLAGS) main.cpp $(LIBS)

clean:
	rm -f main lab7.plt points.txt

exec:
	make all
	./main $(M) $(S)