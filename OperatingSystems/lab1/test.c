#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <time.h>
#include <sys/times.h>
#include <sys/resource.h>
#include <string.h>

#include "list_management.h"

typedef struct {
	double firstReal, firstUser, firstSys;
	double prevReal, prevUser, prevSys;
} prevTimes;

void printTimes(int checkpoint, prevTimes* times) {
	clock_t myClock = clock();

	struct rusage rusage;
	getrusage(RUSAGE_SELF, &rusage);

	double realTime = (double) myClock / CLOCKS_PER_SEC;
	double userTime = (double) rusage.ru_utime.tv_sec + (double) rusage.ru_utime.tv_usec / 10e6;
	double sysTime = (double) rusage.ru_stime.tv_sec + (double) rusage.ru_stime.tv_usec / 10e6;

	if(checkpoint == 1) {
		times->firstReal = realTime;
		times->firstUser = userTime;
		times->firstSys = sysTime;
	}

	printf("%d CHECKPOINT\n", checkpoint);
	printf("- real time: %10.9f s", realTime);
	if(checkpoint > 1) printf(", previous: %10.9f s, first: %10.9f", realTime - times->prevReal, realTime - times->firstReal);
	printf("\n- user time: %10.9f s", userTime);
	if(checkpoint > 1) printf(", previous: %10.9f s, first: %10.9f", userTime - times->prevUser, userTime - times->firstUser);
	printf("\n- syst time: %10.9f s", sysTime);
	if(checkpoint > 1) printf(", previous: %10.9f s, first: %10.9f", sysTime - times->prevSys, sysTime - times->firstSys);
	printf("\n================================\n\n");

	times->prevReal = realTime;
	times->prevUser = userTime;
	times->prevSys = sysTime;
}

void fillListWithRandom(PersonList* list, int loopCounter, void (*addNewPerson)(PersonList* list, char* fullname, char* contactInfo, char* address)) {
	srand(time(NULL));

	char* names[] = { "Maciej Nowak", "Jan Kowalski", "Kamil Musial", "Monika Aba", "Agnieszka Sliwka", "Natalia Mors", "Michal Leszcz", "Boguslaw Karta" };
	char* mails[] = { "zamz at gmail.com", "another at mail.com", "sampleMail at yahoo.pl", "noideawhattotype at gminaborzecin.pl", "illneverpass at warunki.agh.edu.pl" };
	char* cities[] = { "Krakow", "Warszawa", "Poznan", "Miechow", "Wroclaw", "Gdynia", "Gdansk" };

	int pSize = sizeof(char*);

	int nameSize = sizeof(names) / pSize;
	int mailSize = sizeof(mails) / pSize;
	int citySize = sizeof(cities) / pSize;

	int i;
	for(i = 0; i < loopCounter; i++) {
		addNewPerson(list, names[rand() % nameSize], mails[rand() % mailSize], cities[rand() % citySize]);
	}
}

int main() {
	prevTimes* times = malloc(sizeof(prevTimes));
	printTimes(1, times);

	#ifdef DLL
	void* lib_handle;
	lib_handle = dlopen("./dynamicListLib.so", RTLD_LAZY);

	PersonList* (*createList)() = dlsym(lib_handle, "createList");
	void (*addNewPerson)(PersonList* list, char* fullname, char* contactInfo, char* address) = dlsym(lib_handle, "addNewPerson");
	void (*printList)(PersonList* list) = dlsym(lib_handle, "printList");
	void (*sortList)(PersonList* list, int fieldName) = dlsym(lib_handle, "sortList");
	#endif

	PersonList* list = createList();
	fillListWithRandom(list, 40000, addNewPerson);
	printTimes(2, times);

	sortList(list, 1);
	printTimes(3, times);

	free(times);

	#ifdef DLL
	dlclose(lib_handle);
	#endif

	return 0;
}
