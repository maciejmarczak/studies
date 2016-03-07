CC=gcc
MAKE=C:\MinGW\bin\mingw32-make.exe

static: list_management.o
	ar crs staticListLib.a list_management.o
	$(MAKE) cleanObjectFiles

dynamic: list_management.o
	$(CC) -shared -o dynamicListLib.so list_management.o
	$(MAKE) cleanObjectFiles

list_management.o:
	$(CC) -c list_management.c

clean: cleanObjectFiles
	rm -f *.a *.so *.exe

cleanObjectFiles:
	rm -f *.o