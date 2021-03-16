#include "hexdump.c"
#include "page.c"

extern char __bss_start;
extern char __bss_end;
extern char __bss_size;

int global;

void clear_bss();
void kernel_main();

void clear_bss() {

	char * bssStart = &__bss_start;
	char * bssEnd = &__bss_end;
	int bssSize = bssEnd - bssStart;
	for (int i = 0; i < bssSize; i++) {
		bssStart[i] = 0;
	}
}


void kernel_main() {
	clear_bss();

	init_pfa_list();
	struct ppage *test = free_pages;
	struct ppage *test_iterator = test;
	struct ppage *allocated_list;

	int count = 0;
	esp_printf(putc, "printing first 15 pages\n");
	for(int i = 0; i < 15; i++) {
		esp_printf(putc, "Page no: %d Page addr: %x ", ++count, test_iterator);
		esp_printf(putc, "Next: %x Prev: %x Physical addr: %x\n", 
				test_iterator->next, test_iterator->prev, test_iterator->physical_addr);
		test_iterator = test_iterator->next;
	}

	test = allocate_physical_pages(10);
	test_iterator = test;
	count = 0;
	esp_printf(putc, "\n\nprinting 10 allocated pages (should be pages 1-10 from the previous list in reverse order)\n");
	for (int i = 0; i < 10; i++) {
		esp_printf(putc, "Page no: %d Page addr: %x ", ++count, test_iterator);
		esp_printf(putc, "Next: %x Prev: %x Physical addr: %x\n", 
				test_iterator->next, test_iterator->prev, test_iterator->physical_addr);
		test_iterator = test_iterator->next;
	}

	allocated_list = test;
	test = free_pages;
	test_iterator = test;
	count = 0;
	esp_printf(putc, "\n\nprinting first 15 pages (page 11 from the first list should now be page 1)\n");
	for(int i = 0; i < 15; i++) {
		esp_printf(putc, "Page no: %d Page addr: %x ", ++count, test_iterator);
		esp_printf(putc, "Next: %x Prev: %x Physical addr: %x\n", 
				test_iterator->next, test_iterator->prev, test_iterator->physical_addr);
		test_iterator = test_iterator->next;
	}

	free_physical_pages(allocated_list);
	test = free_pages;
	test_iterator = test;
	count = 0;
	esp_printf(putc, "\n\npages have been freed.\nprinting first 15 pages (should be identical to the first list)\n");
	for(int i = 0; i < 15; i++) {
		esp_printf(putc, "Page no: %d Page addr: %x ", ++count, test_iterator);
		esp_printf(putc, "Next: %x Prev: %x Physical addr: %x\n", 
				test_iterator->next, test_iterator->prev, test_iterator->physical_addr);
		test_iterator = test_iterator->next;
	}


	/*
	esp_printf(putc, "hello\n");
	
	esp_printf(putc, "initializing list...");
	init_pfa_list();
	esp_printf(putc, "done\n");

	int n = 3;
	esp_printf(putc, "allocating %d pages...", n);
	struct ppage *list = allocate_physical_pages(n);
	esp_printf(putc, "done\n");
	struct ppage *current = list;
	for (int i = 0; i < n; i++) {
		esp_printf(putc, "dumping page %d (%x)\n", i, current);
		hexdump(current, 12);
		current = current->next;
	}
	esp_printf(putc, "goodbye\n");
	*/
}
