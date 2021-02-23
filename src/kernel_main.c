#include "gpio.c"

extern char __bss_start;
extern char __bss_end;
extern char __bss_size;

int global;

void clear_bss();
void kernel_main();
void led_init();
void led_on();
void led_off();
void sleep(unsigned long);

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
	led_init();
	while (1) {
		led_on();
		delay();
		led_off();
		delay();
	}
}
