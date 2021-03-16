#include "rprintf.c"
#include "serial.c"

void hexdump(char *start, unsigned int length) {
	char *end = start + length;
	char *current = start;
	while (current < end) {
		esp_printf(putc, "%p  ", current);	// print the starting address for this line
		char chars[17] = { 0 };
		// iterate through each char in the next 16 and print the hex value, add the char value to an array
		for (int i = 0; i < 16; i++) {
			if (current + i >= end) { break; }	// check if we are at the end yet
			esp_printf(putc, "%02x ", *(current + i));
			if (i == 7) { esp_printf(putc, " "); }	// extra space for formatting
			char c = *(current + i);
			if (c >= 32 && c <= 126) {	// the range of printable ascii characters (besides delete)
				chars[i] = c;
			} else {			// if the char is not printable, use '.'
				chars[i] = '.';
			}
		}
		esp_printf(putc, " |%s|\n", chars);
		current += 16;
	}
}
