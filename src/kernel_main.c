#include "rprintf.h"
#include "fat.h"
#include "serial.h"
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

	fatInit();
	
	struct file *f;
	fatOpen(f, "TEST.TXT");

	char *data = (char *)fatRead(f);
	esp_printf(putc, "%s\n", data);
}



