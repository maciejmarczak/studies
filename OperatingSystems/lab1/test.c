#include <stdio.h>
#include <stdlib.h>

#include "list_management.h"

int main() {
	PersonList* list = createList();
    addNewPerson(list, "Maciej Marczak", "macmar94 at gmail.com", "Krakow");
    addNewPerson(list, "Adam Nowak", "adamnowak at gmail.com", "Warszawa");
    addNewPerson(list, "Kamil Piech", "kamilpiech at gmail.com", "Poznan");
    addNewPerson(list, "Andrzej Hamak", "andrzhak at gmail.com", "Krolewiec");

    sortList(list, 3);

    printList(list);

    return 0;
}