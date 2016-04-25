all: main reader writer
	./main

main:
	gcc -o main main.c -lrt -lpthread

reader:
	gcc -o reader reader.c -lrt -lpthread

writer:
	gcc -o writer writer.c -lrt -lpthread

clean:
	rm -f writer reader main
