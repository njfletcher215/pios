#include <stdint.h>

int putc(int data) {
	uint32_t *mu_io = (uint32_t *)0x3F215040;
	*mu_io = (uint32_t)data;
	return data;
}
