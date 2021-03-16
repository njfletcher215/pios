#include <stdint.h>

void led_init();
void led_on();
void led_off();

void led_init() {
	uint32_t *gpfsel4 = (uint32_t *)0xFE200010;
	unsigned int andMask = 0xFFFFFE3F;	// 1111 1111 1111 1111 1111 1100 0111 1111
	*gpfsel4 = *gpfsel4 & andMask;
	*gpfsel4 = *gpfsel4 | (1 << 6);
	return;
}

void led_on() {
	uint32_t *gpset1 = (uint32_t *)0xFE200020;
	*gpset1 = (1 << 10);
}

void led_off() {
	uint32_t *gpclr1 = (uint32_t *)0xFE20002C;
	*gpclr1 = (1 << 10);
}

void delay() {
	unsigned int x = 0;
	for (x; x < 100000; x++) {
		asm("NOP");
	}
}

