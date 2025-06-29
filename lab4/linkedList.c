/***********************************************************
* Author: Brayden Daly
* Email: 
* Date Created: 
* Filename: linkedList.c
*
* Overview:
*	This program is an implementation of the linked list and the deque ADTs. 
*
************************************************************/

#include "linkedList.h"


/* ************************************************************************
	Linked List Functions
************************************************************************ */

//This function creates a Linked List
LinkedList* linkedListCreate()
{
	//create space in memory for the linked list
	LinkedList* mylist = (LinkedList*) malloc(sizeof(LinkedList));
	//initialize the head and tail to null and the size to zero
	mylist->head = NULL;
	mylist->tail = NULL;
	mylist->size = 0;
	return mylist;

}

//This function deletes the list link by link and frees each link from memory space
void deleteList( LinkedList* myList )
{
	//create a tracker link called current to keep track of which node we are looking at and make that current node the tail node
	Link* current = myList->tail;
	//loop through the linked list backwards, and free the current link from memory
	//make the current link the previous one to move packward in the link
	while (current != NULL )
	{
		//make a temporary link to hold the link before the current link
		Link* prev = current->previous;
		//free the current link
		free(current);
		//go to previous node in linked list
		current = prev;
	}

	//make the head, tail of linked list NULL and the size of the Linked List zero
	myList->head = NULL;
	myList->tail = NULL;
	myList->size = 0;

	//free the linked list and make it null to stop memory leak
	free(myList);
	myList = NULL;

}
							

//return the size of the linked list
int sizeList( LinkedList* myList )
{
	return myList->size;
}						


//iterate through the linked list and if the value value is in the linked list, return the index of the value, otherwise return -1
int containsList( LinkedList* list, TYPE value )
{
	//create a tracker link and start it at the head of the linked list
	Link* current = list->head;
	//make a counter for the index of the linked list
	int idx = 0;
	while (current != NULL)
	{
		//accumulate the index until the value is reached then return the index value
		if (current->value == value )
		{
			return idx;
		}
		idx ++;
		current = current->next;
	}
	
	//if no value exists, return -1
	return -1;
}						


//This function adds a new link with a value to the front of the linked list

void addListFront( LinkedList* list, TYPE value )
{
	//Initialize new node to add with value in it
	//Free space for new link using malloc
	//set value to value, and next and previous nodes to NULL
	
	//if the list is empty, return
	if (list == NULL)
	{
		return;
	}
	
	//create a new link and initialize the next, previous to null and put the value into it
	Link* newlink = (Link*) malloc(sizeof(Link));
	newlink->next = NULL;
	newlink->previous = NULL;
	newlink->value = value;


	//check if linked list is empty
	if(list->size == 0)
	{
		list->head=newlink;
		list->tail=newlink;
	}
	else
	{
		//make the link before head the newlink
		//make the original head the next to the  newlink
		//make the newlink the head of the linked list
		//increase the size
		list->head->previous = newlink;
		newlink->next = list->head;
		list->head = newlink;
	}
	list->size ++;




}		

//add a value in the back of the linked list
void addListBack( LinkedList* list, TYPE value )
{
	//Initialize new node to add with value in it
	//Free space for new link using malloc
	//set value to value, and next and previous nodes to NULL
	//set the size to 0
	Link* newlink = (Link*) malloc(sizeof(Link));
	newlink->next = NULL;
	newlink->previous = NULL;
	newlink->value = value;

	//if there is no head link, make the newlink the head link and set the value to the parameter value, make the size 1
	if (list->head == NULL)
	{
		list->head = newlink;
		list->tail = newlink;
	}
	else
	{
		//current link is the tail link
		Link* current_link = list->tail;
		//make the link after the tail link the newlink
		current_link->next = newlink;
		//make the newlink point in reverse to the currentlink
		newlink->previous = current_link;
		//make the tail of the linked list the tail
		list->tail = newlink;


	}

	//increment the linked list
	list->size ++;

}	


//add a value to the front of the linked list
void addValueListFront( LinkedList* list, TYPE value, TYPE valueToAdd ){
	//iterate until we find the value, then when we find the value, make the previous node's next node the current node's next node and vice versa
	//create a current tracker link the head of the linked list, and a newlink with its value and null next and previous pointers
	Link* current = list->head;
	Link* newlink = (Link*) malloc(sizeof(Link));
	newlink->value = valueToAdd;
	newlink->next = NULL;
	newlink->previous = NULL;

	//if there are no links in the linked list make the newlink the head of the linked list
	if ( list->size == 0 ){
		list->head = newlink;
		newlink->value = valueToAdd;
	}

	//if the head value is contains the value, add another link before with the newvalue to add
	else if ( list->head->value == value )
	{
		current->previous = newlink;
		list->head = newlink;	
		newlink->next = current;
		newlink->value = valueToAdd;
	}

	//otherwise, if the tail contains the value, add another link after the tail and make that the new tail
	else if ( list->tail->value == value )
	{
		current = list-> tail;
		current->previous = newlink;
		newlink->next = current;
		newlink->value = valueToAdd;
	}
	//otherwise, iterate through the linked list and find the value and insert a node before with the value
	else
	{
		while( current != NULL )
		{
			if (current->value == value)
			{
				Link* temp = current->previous;
				current->previous->next = newlink;
				newlink->next = current;
				newlink->previous = temp;
				newlink->value = valueToAdd;
				list->size++;
				return;
			}
			current = current->next;
		}
	}

	//increment the linkedlist size
	list->size ++;

}	
void addValueListBack( LinkedList* list, TYPE value, TYPE valueToAdd ){
	//iterate until we find the value, then when we find the value, make the next node's next node the current node's next node and vice versa
	Link* current = list->head;
	Link* newlink = (Link*) malloc(sizeof(Link));
	newlink->value = valueToAdd;
	newlink->next = NULL;
	newlink->previous = NULL;

	//if there are no links in the linked list make the newlink the head of the linked list

	if ( list->size == 0 ){
		list->head = newlink;
		newlink->value = valueToAdd;
	}
	//if the head value is contains the value, add another link after with the newvalue to add

	else if ( list->head->value == value )
	{
		newlink->next = current->next;
		newlink->previous = current;
		current->next->previous = newlink;
		current->next = newlink;
		newlink->value = valueToAdd;
	}
	
	//otherwise, if the tail contains the value, add another link before the tail and make that the new tail

	else if ( list->tail->value == value )
	{
		current = list-> tail;
		current->next = newlink;
		newlink = list->tail;
		newlink->value = valueToAdd;
	}

	//otherwise, iterate through the linked list and find the value and insert a node after with the value

	else
	{
		while( current != NULL )
		{
			if (current->value == value)
			{
				Link* temp = current->next;
				newlink->next = temp;
				current->next = newlink;
				newlink->previous = current;
				newlink->value = valueToAdd;
				list->size++;
				return;
			}
			current = current->next;
		}
	}
	//accumulate the size of the linked list
	list->size ++;
}	

void removeList(LinkedList* list, TYPE value) {
    if (list == NULL || list->head == NULL) return;  // Check for empty list
	//make a current link tracker and have that be the head of the linked list
    Link* current = list->head;

	//iterate until the value is found in a node
    while (current != NULL) {
        if (current->value == value) {
            // if it is not the head, make the node before current nude point to the node after current node
            if (current->previous != NULL) 
			{
                current->previous->next = current->next;
            } 
			else 
			{
                list->head = current->next;  // Update head if necessary
            }

            // if it is not the tail, make the node after current nude point to the node before current node
            if (current->next != NULL) 
			{
                current->next->previous = current->previous;
            }
			else 
			{
                list->tail = current->previous;  // Update tail if necessary
            }
			
			//free the current node of the linked list
            free(current);
            list->size--;
            return;  // Stop after first match is found
        }
		//move on to the next node
        current = current->next;
    }
}

void printList( LinkedList* list )
{

	//create a variable current to keep track of where we are in the linked list
	Link* current = list->head;

	//if there is only one link, print the head
	if( list->size == 1)
	{
		printf("%d", current->value);
	}
	//if there are multiple links, iterate through them and print each value with an arrow
	else if ( list->size > 1)
	{
		//iterate through nodes printing each node value adn an arrow after
		while ( current->next != NULL )
		{
			printf("%d", current->value);
			printf(" -> ");
			current = current->next;
		}
		//print the tail of the linked list
		printf("%d", current->value);
	}	
	//if the size is not 0, print a new line
	if (list->size != 0)
	{
		printf("\n");
	}
}		

//this function reverses the linked list
void reverseList( LinkedList* list )
{
	//if only the head exists, do nothing
	if ( list->size == 1){
		return;
	}
	//store the head
	//store the tail
	//iterate through list and make the previous the next and vice versa
	Link* current = list->head;
	Link* temp = NULL;

	//iterate through the linked list
	while ( current != NULL )
	{
		//create a temp variable to hold the node before the current node
		temp = current->previous;
		//reverse the direction of the pointers
		current->previous = current->next;
		current->next = temp;
		current = current->previous;
	}
	//make the last node the head and the first node the tail
	temp = list->head;
	list->head = list->tail;
	list->tail = temp;
	current = NULL;
}										



/* ************************************************************************
	Deque Functions
************************************************************************ */
//use the linked list create to create a deque
Deque* dequeCreate()
{
	return linkedListCreate();
}				
//use the delete list to delete the queue		
void deleteDeque(Deque* myDeque)
{
	deleteList(myDeque);

}	
//use the size list to find the size of the deque		
int sizeDeque(Deque* myDeque)
{
	return sizeList(myDeque);
}		

//use addListFront to add to the front of the deque
void addFrontDeque(Deque* myDeque, TYPE value)
{
	addListFront(myDeque, value);
}	

//return the value of the tail of the Deque
TYPE backDeque(Deque* myDeque)
{
	return myDeque->tail->value;
}	

//removes value at the back of the dequeue	
//free the tail of the linkedlist	
//set the pointer of second to last value to point to NULL and make it 
void removeBackDeque(Deque* myDeque)
{
	//if there is no tail node, return nothing because the deque is empty
    if (myDeque->tail == NULL) {
        fprintf(stderr, "Error: Deque is empty.\n");
        return;
    }

	//create a temporary link and make that the last node of the deque
    Link* temp = myDeque->tail;
	//make the deque tail the node before the tail
    myDeque->tail = myDeque->tail->previous;
	//if the tail is not null, make the node after the tail null
    if (myDeque->tail != NULL) {
        myDeque->tail->next = NULL;
    } 
	//otherwise, make the head null
	else
	{
        myDeque->head = NULL;
    }
	//free the temporary variable and decrement the size
    free(temp);
    myDeque->size--;
}

//use the printlist function to print the deque
void printDeque(Deque* myDeque)
{
	printList(myDeque);
}