#include "rprintf.h"

int strcmp(char *s1, char *s2) {
	while (1) {
		char c1 = *s1;
		char c2 = *s2;
		if (c1 == '\0') {
			break;
		}
		if (c1 != c2) {
			return 0;
		}	
		s1++;
		s2++;
	}
	return 1;
}
	
