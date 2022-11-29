#include "./buffy.h"
#include "./keys.c"

char *IOBUFF = {0};

int main ( int argc, char** argv ) {
	IOBUFF = malloc(6);
	struct fbjar jar = InitBuffy();
	InitTerm(jar);
	setbuffer(stdin, IOBUFF, 9);
	char k = getc(stdin);
	printf("k:%c\nbf:", k);

	int i;
	for (i = 0; i<4; i++) {
		printf("%i, ", IOBUFF[i]);
	}
	printf("%i\n", IOBUFF[i]);
	printf("%li\n", compact(IOBUFF));

	CloseTerm(jar);
	CloseFb(jar);
	ShowCursor();
	return 0;
}
