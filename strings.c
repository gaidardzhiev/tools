#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MIN 4

void s(FILE *f) {
	int c;
	int n = 0;
	char b[1024];
	while ((c = fgetc(f)) != EOF) {
		if (isprint(c)) {
			b[n++] = c;
			if (n >= sizeof(b) - 1) {
				break;
			}
		} else {
			if (n >= MIN) {
				b[n] = '\0';
				printf("%s\n", b);
			}
			n = 0;
		}
	}
	if (n >= MIN) {
		b[n] = '\0';
		printf("%s\n", b);
	}
}

int main(int z, char *argv[]) {
	if (z < 2) {
		fprintf(stderr, "usage: %s <file>\n", argv[0]);
		return EXIT_FAILURE;
	}
	for (int i = 1; i < z; i++) {
		FILE *f = fopen(argv[i], "r");
		if (!f) {
			perror("error opening file");
			continue;
		}
		printf( argv[i]);
		s(f);
		fclose(f);
	}
	return EXIT_SUCCESS;
}
