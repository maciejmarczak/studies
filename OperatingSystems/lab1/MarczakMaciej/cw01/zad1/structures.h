#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef struct {
    char* fullname;
    char* contactInfo;
    char* address;
} Person;

typedef struct PersonElement {
    Person* person;

    struct PersonElement* prev;
    struct PersonElement* next;
} PersonElement;

typedef struct {
    int size;

    PersonElement* head;
} PersonList;

#endif
