CC=gcc
MAKE=C:\MinGW\bin\mingw32-make.exe

static: main.o
	ar crs staticListLib.a main.o
	$(MAKE) cleanObjectFiles

dynamic: main.o
	$(CC) -shared -o dynamicListLib.so main.o
	$(MAKE) cleanObjectFiles

main.o:
	$(CC) -c main.c

clean: cleanObjectFiles
	rm -f *.a *.so

cleanObjectFiles:
	rm -f *.o