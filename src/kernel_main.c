extern char __bss_start;
extern char __bss_end;
extern char __bss_size;

int global ;

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
	while (1) {
	}
}
