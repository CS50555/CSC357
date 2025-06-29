#include "linkedList.h"

int main(int argc, char* argv[])
{
	
	// Write test cases for your linked list and deque
	printf("LINKED LIST TESTING\n");
	LinkedList* mylist = linkedListCreate();
	addListBack(mylist, 5);
	printList(mylist);
	//5
	addListBack(mylist, 10);
	printList(mylist);
	//5 -> 10
	deleteList(mylist);
	mylist = linkedListCreate();
	printList(mylist);
	// NULL
	addListFront(mylist, 6);
	printList(mylist);
	//6 
	addListFront(mylist, 5);
	printList(mylist);
	//5 -> 6 
	addValueListBack(mylist, 5, 3);
	printList(mylist);
	//5 -> 3 -> 6
	addValueListFront(mylist, 3, 11);
	printList(mylist);
	//5 -> 11 -> 3 -> 6
	removeList(mylist, 11);
	printList(mylist);
	//5 -> 3 -> 6
	int temp = sizeList(mylist);
	printf("Current Size of List: %d\n", temp);
	// Current Size of List: 3
	temp = containsList(mylist, 3);
	printf("Index of value: %d\n", temp);
	// Index of Value: 1
	reverseList(mylist);
	printList(mylist);
	//6 -> 3 -> 5
	deleteList(mylist);

	printf("DEQUE TESTING\n");

	Deque* dq = dequeCreate();
	addFrontDeque(dq, 5);
	printDeque(dq);
	//5
	addFrontDeque(dq, 7);
	addFrontDeque(dq, 2);
	printDeque(dq);
	//2-> 7 -> 5
	temp = sizeDeque(mylist);
	printf("Current Size of Deque: %d\n", temp);
	//Current Size of Deque: 3 
	temp = backDeque(dq);
	printf("Last value of Deque: %d\n", temp);
	//Last value of Deque: 5
	removeBackDeque(dq);
	printDeque(dq);
	//2 -> 7
	deleteDeque(dq);
	printDeque(dq);
	// NULL
	return 0;
	

}

