#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structures.h"

PersonList* createList() {
    PersonList* list = (PersonList*) malloc(sizeof(PersonList));

    list->head = NULL;
    list->size = 0;

    return list;
}

void deleteList(PersonList* list) {
    PersonElement* pEl = list->head;
    PersonElement* tmp;

    while(pEl != NULL) {
        free(pEl->person);
        tmp = pEl->next;
        free(pEl);
        pEl = tmp;
    }

    free(list);
}

void addNewPerson(PersonList* list, char* fullname, char* contactInfo, char* address) {
    Person* p = (Person*) malloc(sizeof(Person));

    p->fullname = fullname;
    p->contactInfo = contactInfo;
    p->address = address;

    PersonElement* pEl = (PersonElement*) malloc(sizeof(PersonElement));

    pEl->person = p;
    pEl->prev = NULL;

    // case: list is empty
    if(list->head == NULL) {
        pEl->next = NULL;
    }
    // case: list not empty
    else {
        pEl->next = list->head;
        list->head->prev = pEl;
    }

    list->head = pEl;
    list->size++;
}

void printList(PersonList* list) {
    PersonElement* current = list->head;

    while(current != NULL) {
        printf("|%25s|\n", current->person->fullname);
        printf("|%25s|\n", current->person->contactInfo);
        printf("|%25s|\n\n\n", current->person->address);

        current = current->next;
    }
}

void deletePerson(PersonList* list, Person* person) {
    PersonElement* currentElement = list->head;

    while(currentElement != NULL && currentElement->person != person) {
        currentElement = currentElement->next;
    }

    if(currentElement == NULL) {
        printf("Couldn't find given person in the list.\n");
        return;
    }

    // case: the only element
    if(list->size == 1) {
        list->head = NULL;
    }
    // case: first element
    else if(currentElement->prev == NULL) {
        list->head = list->head->next;
        list->head->prev = NULL;
    }
    // case: last element
    else if(currentElement->next == NULL) {
        currentElement->prev->next = NULL;
    }

    free(currentElement->person);
    free(currentElement);

    list->size--;
}

Person* findPerson(PersonList* list, char* fullname, char* contactInfo, char* address) {
    PersonElement* current = list->head;
    Person* p;

    while(current != NULL) {
        p = current->person;

        int namesEquality = strcmp(p->fullname, fullname);
        int contactEquality = strcmp(p->contactInfo, contactInfo);
        int addressEquality = strcmp(p->address, address);

        if(!(namesEquality || contactEquality || addressEquality)) {
            return p;
        }

        current = current->next;
    }

    return NULL;
}

void sortList(PersonList* list, int fieldNumber) {
    if(list->size < 2) return;

    // starting with 2nd element
    PersonElement* currentlyInserted = list->head->next;

    // helper variables
    PersonElement *prev, *prevNext, *next;

    while(currentlyInserted != NULL) {
        // predecessor and successor of currently inserted node
        prev = currentlyInserted->prev;
        next = currentlyInserted->next;

        // unhook element
        prev->next = prevNext = currentlyInserted->next;
        if(currentlyInserted->next != NULL) currentlyInserted->next->prev = prev;

        while(prev != NULL && comparePersons(prev->person, currentlyInserted->person, fieldNumber) > 0) {
            prevNext = prev;
            prev = prev->prev;
        }

        if(prev == NULL) {
            // inserting as head
            currentlyInserted->prev = NULL;
            currentlyInserted->next = prevNext;
            prevNext->prev = currentlyInserted;
            list->head = currentlyInserted;
        }
        else if(prevNext == NULL) {
            // last element
            currentlyInserted->next = NULL;
            currentlyInserted->prev = prev;
            prev->next = currentlyInserted;
        }
        else {
            // normal case
            currentlyInserted->next = prevNext;
            prevNext->prev = currentlyInserted;
            currentlyInserted->prev = prev;
            prev->next = currentlyInserted;
        }

        currentlyInserted = next;
    }
}

int comparePersons(Person* p1, Person* p2, int fieldNumber) {
    if(fieldNumber == 1) {
        return strcmp(p1->fullname, p2->fullname);
    }
    else if(fieldNumber == 2) {
        return strcmp(p1->contactInfo, p2->contactInfo);
    }
    else {
        return strcmp(p1->address, p2->address);
    }
}

/*int main() {
    PersonList* list = createList();
    addNewPerson(list, "Maciej Marczak", "macmar94 at gmail.com", "Krakow");
    addNewPerson(list, "Adam Nowak", "adamnowak at gmail.com", "Warszawa");
    addNewPerson(list, "Kamil Piech", "kamilpiech at gmail.com", "Poznan");
    addNewPerson(list, "Andrzej Hamak", "andrzhak at gmail.com", "Krolewiec");

    sortList(list, 3);

    printList(list);

    return 0;
}*/
