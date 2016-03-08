CC=gcc

static: staticLib
	$(CC) -o test test.c staticListLib.a
	./test

dynamic: dynamicLib
	$(CC) -o test test.c ./dynamicListLib.so
	./test

dynamicDLL: dynamicLib
	$(CC) -o test test.c -DDLL -rdynamic -ldl
	./test

staticLib: list_static
	ar crs staticListLib.a list_management.o
	$(MAKE) cleanObjectFiles

dynamicLib: list_dynamic
	$(CC) -shared -fPIC -o dynamicListLib.so list_management.o
	$(MAKE) cleanObjectFiles

list_static:
	$(CC) -c list_management.c

list_dynamic:
	$(CC) -c -fPIC list_management.c

clean: cleanObjectFiles
	rm -f *.a *.so
	rm test

cleanObjectFiles:
	rm -f *.o