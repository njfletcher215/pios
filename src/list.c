#include "list.h"

int list_add(int);
int list_remove(int);

list_element *head = NULL;

int list_add(int data) {

	// the element we are adding to the end of the list
	struct list_element new = { NULL, data };

	if (head == NULL) {
		head = new;	// if this is the first element in the list, it becomes head
	} else {		// else we will have to traverse the list
		struct list_element current = head;
		while (current.next != NULL) {	// reach the last element in the list
			current = current.next;
		}

		current.next = &new;	// last element's next becomes address of new
	}
	return 0;
}

// returns the number of times the data was removed from the list (0 if data did not exist)
int list_remove(int data) {
	
	int rmCount = 0;

	// first check to see if head exists
	if (head == NULL) {
		return rmCount;
	}
	// special case if the head is to be removed
	while (head.data == data) {
		rmCount++;
		head = head.next;	// we can simply move head (this will null head if we reach the end of the list)
	}

	struct list_element current = head;

	// start on an element we know is safe, look ahead to the next element
	while (current != NULL) {
		// we use a while loop here because we have to keep checking current.next.data until we know it is not data,
		// since current must be safe
		while (current.next != NULL && current.next.data == data) {
			// check current.next != NULL to make sure that current.next.data doesn't throw an error,
			// then look ahead to current.next.data to see if it is data
			rmCount++;
			current.next = current.next.next;	// cuts the element out, then look again
		}	// now we know current.next is either NULL or safe
		current = current.next; // continue to the next safe element or end the loop
	}
	return rmCount;
}

