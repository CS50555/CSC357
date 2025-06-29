#ifndef LINKED_LIST_H
#define LINKED_LIST_H 1

#ifndef TYPE
#define TYPE int
#endif

#ifndef FORMAT_SPECIFIER
#define FORMAT_SPECIFIER "%d"
#endif

#ifndef LT
#define LT(A, B) ((A) < (B))
#endif

#ifndef GT
#define GT(A, B) ((A) > (B))
#endif

#ifndef EQ
#define EQ(A, B) ((A) == (B))
#endif

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "linkedList.h"


typedef struct LinkedList 		// Creates the alliases LinkedList and Deque for the struct LinkedList
{
	struct Link* head;
	struct Link* tail;
	int size;
}LinkedList, Deque;


typedef struct Link				// Creates the allias Link for the struct Link
{
	TYPE value;
	struct Link* previous;
	struct Link* next;
}Link;


/* ************************************************************************
	Linked List Interface
************************************************************************ */

LinkedList* linkedListCreate();												// Allocates and initializes a doubly linked list with head and tail pointers
void deleteList( LinkedList* myList );										// Deallocates every link in the list including the sentinels and deletes the list itself
int sizeList( LinkedList* myList );											// Returns the size of the list
int containsList( LinkedList* list, TYPE value );							// Returns the index of the link if value is in the list otherwise -1
void addListFront( LinkedList* list, TYPE value );							// Adds a new link at the front of the list and increments the list's size
void addListBack( LinkedList* list, TYPE value );							// Adds a new link at the back of the list and increments the list's size
void addValueListFront( LinkedList* list, TYPE value, TYPE valueToAdd );	// Adds a new link at the before an existing value in the list and increments the list's size
void addValueListBack( LinkedList* list, TYPE value, TYPE valueToAdd );		// Adds a new link at the after an existing value in the list and increments the list's size
void removeList( LinkedList* list, TYPE value );							// Removes the given value from the list and decrements the list's size
void printList( LinkedList* list );											// Prints the values of the links in the list from front to back
void reverseList( LinkedList* list );										// Reverse the list


/* ************************************************************************
	Deque Interface
************************************************************************ */

Deque* dequeCreate();							// Allocates and initializes the deque
void deleteDeque(Deque* myDeque);				// Deallocates the data and deletes the deque
int sizeDeque(Deque* myDeque);					// Returns the size of the deque
void addFrontDeque(Deque* myDeque, TYPE value);	// Adds a new link with the given value to the front of the deque
TYPE backDeque(Deque* myDeque);					// Returns the value of the link at the back of the deque
void removeBackDeque(Deque* myDeque);			// Removes the link at the back of the deque
void printDeque(Deque* myDeque);				// Prints the values of the links in the deque from front to back


#endif