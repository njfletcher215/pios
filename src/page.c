#include "page.h"
#include "list.h"
#define NPAGES 128
#define PAGE_SIZE 0x200000
#define NULL (void *)0

extern int __end; // defined in the linker

struct ppage physical_page_array[NPAGES]; // 128 pages, each 2mb in length covers 256 megs of memory
struct ppage *free_pages = NULL; // the head of the free_pages list

// initializes free_pages
void init_pfa_list(void) {
	// align the pages on blocks of size PAGE_SIZE, the first of which should be right after __end
	void *address_p = (((unsigned long)&__end) & ~(PAGE_SIZE- 1)) + PAGE_SIZE;

	// add every page in physical_page_array to free_pages list	
	for (int i = 0; i < NPAGES; i++) {
		listAdd(&free_pages, (struct listElement*) &physical_page_array[i]);
		physical_page_array[i].physical_addr = address_p;
		address_p += PAGE_SIZE;
	}
	
	// nullify the head's prev so we know not to follow it
	free_pages->prev = NULL;
}

// allocate npages number of pages and return the head to a linked list containing those pages
struct ppage *allocate_physical_pages(unsigned int npages) {
	struct ppage *page_to_add;
	struct ppage *new_free_head; // keep track of what the head of free_pages list should be
	struct ppage *allocated_list = NULL;
	for (int i = 0; i < npages; i++) {
		page_to_add = free_pages; // we are about to add the head of the free_pages list
		new_free_head = free_pages->next; // save free_pages->next, this will be the new head of free_pages

		listRemove(page_to_add); // remove page_to_add from its old list (free_pages list)
		listAdd(&allocated_list, page_to_add); // and add it to its new list (allocated_list list)

		free_pages = new_free_head; // restore the var free_pages to the head of free_pages list
	}
	return allocated_list;
}

// free all physical pages in a given list (basically the opposite of allocate_physical_pages, but doesnt return free_pages
void free_physical_pages(struct ppage *ppage_list){
	struct ppage *page_to_add; // page to add back to free list
	struct ppage *new_list_head; // keep track of what the head of ppage_list should be
	while (ppage_list != NULL) {		
		page_to_add = ppage_list;
		new_list_head = ppage_list->next;

		listRemove(page_to_add);
		listAdd(&free_pages, page_to_add);

		ppage_list = new_list_head;
	}
}
