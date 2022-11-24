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

// include draw funcs
#include "draw.c"
// include Advanced draw funcs
#include "adraw.c"


int main ( void ) {
	HideCursor();
	struct fbjar jar = InitFb();
	//InitTerm(jar);

	fprintf(jar.log, "fb size: line cols:%d, rows: %d\n\n", jar.cols-1, jar.rows-1);
	fflush(jar.log);


	color white = RGB(255,255,255);
	color prple = RGB(127,0,255);
	color green = RGB(0,255,0);
	int y = 30;
	int thick = 7;

	DrawPartCollum(jar, y, 100, 500, green);
	DrawPartCollum(jar, y-thick, 50, 200, green);
	DrawPartCollum(jar, y+thick, 50, 200, green);
	ADrawPartCollum(jar, y, 100, 400, white, thick);
	ADrawPartCollum(jar, y, 100, 400, prple, -thick);

	getc(stdin);

	//CloseTerm(jar);
	// close framebuffer jar
	CloseFb(jar);
	ShowCursor();
	return 0;
}



