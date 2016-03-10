CC = gcc

static: staticLib
	$(CC) -Wall -o test test.c ../zad1/staticListLib.a
	./test

dynamic: dynamicLib
	$(CC) -Wall -o test test.c ../zad1/dynamicListLib.so
	./test

dynamicDLL: dynamicLib
	$(CC) -Wall -o test test.c -DDLL -rdynamic -ldl
	./test ../zad1/dynamicListLib.so

staticLib:
	cd ../zad1 && make staticLib

dynamicLib:
	cd ../zad1 && make dynamicLib

clean:
	rm test