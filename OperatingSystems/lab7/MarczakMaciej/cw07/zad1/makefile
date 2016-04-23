all: consumer producent main
	./main

consumer: consumer.o common_functions.o
	gcc -o consumer consumer.o common_functions.o

producent: producent.o common_functions.o
	gcc -o producent producent.o common_functions.o

main:
	gcc -o main main.c

producent.o:
	gcc producent.c -o producent.o -c

consumer.o:
	gcc consumer.c -o consumer.o -c

common_functions.o:
	gcc common_functions.c -o common_functions.o -c

clean:
	rm -f *.o producent consumer main
