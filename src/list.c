#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "list.h"


struct node *head = NULL;
struct node *current = NULL;

//display the list
void printList() {
   struct node *ptr = head;
   printf("\n[ ");
	
   //start from the beginning
   while(ptr != NULL) {
      printf("(%d) ", ptr->data);
      ptr = ptr->next;
   }
	
   printf(" ]");
}

//insert link at the first location
void insert(int data) {
   //create a link
   struct node *link = (struct node*) malloc(sizeof(struct node));
	
   link->data = data;
	
   //point it to old first node
   link->next = head;
	
   //point first to new first node
   head = link;
}

//delete first item
struct node* delete() {

   //save reference to first link
   struct node *tempLink = head;
	
   //mark next to first link as first 
   head = head->next;
	
   //return the deleted link
   return tempLink;
}

void main() {
   insert(1);
   insert(2);
   insert(3);
   insert(4);
   insert(5);

   printf("Original List: "); 
	
   //print list
   printList();

   delete();
   delete();
   delete();
	
   printf("\nList after deleting 3 items: ");
   printList();
}