int clear_bss();

void kernel_main() {

	clear_bss();

	while(1){
	}
}

int clear_bss() {
	char *begin_bss = &__bss_start;
	char *end_bss = &__bss_end;
	char *n = begin_bss;

	while(n < end_bss) {
		*n = 0;
		n += 4;
	}

	return 0;
}
