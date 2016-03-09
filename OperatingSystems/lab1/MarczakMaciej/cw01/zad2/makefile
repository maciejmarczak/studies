CC=gcc

static: staticLib
	$(CC) -o test test.c ../zad1/staticListLib.a
	./test

dynamic: dynamicLib
	$(CC) -o test test.c ../zad1/dynamicListLib.so
	./test

dynamicDLL: dynamicLib
	$(CC) -o test test.c -DDLL -rdynamic -ldl
	./test ../zad1/dynamicListLib.so

staticLib:
	cd ../zad1 && make staticLib

dynamicLib:
	cd ../zad1 && make dynamicLib

clean:
	rm test