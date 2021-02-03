extern char __bss_start[];
extern char __bss_end[];
extern char __bss_size[];
void clear_bss() {

	for (int i = 0; i < __bss_size; i++) {
		__bss_start[i] = 0;
	}
}
void kernel_main() {

	while (1) {
		;
	}
}
