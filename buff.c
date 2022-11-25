#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>

#include <assert.h>
#include <time.h>
#include <fcntl.h>
#include <termios.h>

// include initers / funcs
#include "vars.c"

#include "draw/simple.c"
#include "draw/normal.c"
#include "draw/chars.c"


int main ( void ) {
	HideCursor();
	struct fbjar jar = InitFb();
	fprintf(jar.log, "fb size: line cols:%d, rows: %d\n", jar.cols-1, jar.rows-1);
	fprintf(jar.log, "tty: %s\n", jar.tty);
	uint8* chars = ReadChars(1);
	fprintf(jar.log, "chars loaded from 'draw/font'\n");


	//
	bool* chr = ExpandMappedChar(chars, 0);

	for (int i = 0 ; i<CHARLEN; i++) {
		for (int j = 0 ; j<CHARLINELEN ; j++) {
			fprintf(jar.log, "%s", *(chr+i*CHARLEN+j)?"@":".");
		}
		fprintf(jar.log, "\n");
	}
	DrawChar(jar, chr, 2, 2);

	//for (int i = 0 ; i<CHARLEN ; i++) {
	//	printf("%d:%d\n", i, *(chars+i));
	//}

	//

	//CloseTerm(jar);
	// close framebuffer jar
	CloseFb(jar);
	ShowCursor();
	return 0;
}



