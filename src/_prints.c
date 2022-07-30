#include <stdio.h>

// for bold: \33[0;31m -> \33[1;31m

void black(const char *input) {
	printf("\033[0;30m");
	printf("%s\n", input);
	printf("\033[0m");
}


void red(const char *input) {
	printf("\033[0;31m");
	printf("%s\n", input);
	printf("\033[0m");
}


void green(const char *input) {
	printf("\033[0;32m");
	printf("%s\n", input);
	printf("\033[0m");
}


void yellow(const char *input) {
	printf("\033[0;33m");
	printf("%s\n", input);
	printf("\033[0m");
}


void blue(const char *input) {
	printf("\033[0;34m");
	printf("%s\n", input);
	printf("\033[0m");
}


void purple(const char *input) {
	printf("\033[0;35m");
	printf("%s\n", input);
	printf("\033[0m");
}


void cyan(const char *input) {
	printf("\033[0;36m");
	printf("%s\n", input);
	printf("\033[0m");
}


void white(const char *input) {
	printf("\033[0;37m");
	printf("%s\n", input);
	printf("\033[0m");
}
