#ifndef LIST_MANAGEMENT_H
#define LIST_MANAGEMENT_H

#include "structures.h"

PersonList* createList();
void deleteList(PersonList* list);
void addNewPerson(PersonList* list, char* fullname, char* contactInfo, char* address);
void printList(PersonList* list);
void deletePerson(PersonList* list, Person* person);
Person* findPerson(PersonList* list, char* fullname, char* contactInfo, char* address);
void sortList(PersonList* list, int fieldNumber);
int comparePersons(Person* p1, Person* p2, int fieldNumber);

#endif