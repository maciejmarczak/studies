CC = gcc

staticLib: clean list_static
	ar crs staticListLib.a list_management.o
	$(MAKE) cleanObjects

dynamicLib: clean list_dynamic
	$(CC) -shared -fPIC -o dynamicListLib.so list_management.o
	$(MAKE) cleanObjects

list_static:
	$(CC) -Wall -c list_management.c

list_dynamic:
	$(CC) -Wall -c -fPIC list_management.c

clean:
	rm -f *.a *.so *.o

cleanObjects:
	rm -f *.o