#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "list_management.h"

int main() {

	#ifdef DLL
	void* lib_handle;
	lib_handle = dlopen("./dynamicListLib.so", RTLD_LAZY);

	PersonList* (*createList)() = dlsym(lib_handle, "createList");
	void (*addNewPerson)(PersonList* list, char* fullname, char* contactInfo, char* address) = dlsym(lib_handle, "addNewPerson");
	void (*printList)(PersonList* list) = dlsym(lib_handle, "printList");
	#endif

	PersonList* list = createList();
	addNewPerson(list, "Maciej Marczak", "macmar94 at gmail.com", "Krakow");
	addNewPerson(list, "Adam Nowak", "adamnowak at gmail.com", "Warszawa");
	addNewPerson(list, "Kamil Piech", "kamilpiech at gmail.com", "Poznan");
	addNewPerson(list, "Andrzej Hamak", "andrzhak at gmail.com", "Krolewiec");

	printList(list);

	#ifdef DLL
	dlclose(lib_handle);
	#endif

	return 0;
}
