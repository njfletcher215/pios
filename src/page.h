#ifndef _PAGE_H_
#define _PAGE_H_

struct ppage {
	struct ppage *next;
	struct ppage *prev;
	void *physical_addr;
};

#endif
