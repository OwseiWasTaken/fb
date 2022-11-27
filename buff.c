// fb
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
// work
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>
#include <termios.h>
#include <time.h>
// code
#include <stdbool.h>
#include <assert.h>
#include <fcntl.h>
// include initers / funcs
#include "vars.c"
// include drawers
#include "draw/simple.c"
#include "draw/normal.c"
#include "draw/complex.c"
#include "draw/chars.c"

struct fbjar GlobalJar;

void HandleInt( int sig ) {
	ShowCursor();
	CloseFb(GlobalJar);
	exit(0);
}

int main ( void ) {
	signal(SIGINT, HandleInt);

	HideCursor();
	GlobalJar = InitFb();
	struct fbjar jar = GlobalJar;
	fprintf(jar.log, "fb size: line cols:%d, rows: %d\n", jar.cols-1, jar.rows-1);
	fprintf(jar.log, "tty: %s\n", jar.tty);
	//uint8* chars = ReadChars(1);
	//fprintf(jar.log, "chars loaded from 'draw/font'\n");

	time_t rn;
	time(&rn);

	fmttime now = FmtTime(rn);

	color white = RGB(255,255,255);
	color ColSec = RGB(255,0,0);
	color ColMin = RGB(255,0,255);
	color ColHr = RGB(0,0,0);

	// max pointer len
	int MaxR = 250;
	int pt = 350;
	point top = MakePoint(pt-MaxR,pt-MaxR);
	point mid = MakePoint(pt,pt);

	SFillCircle(jar, top, MaxR);

	polar PolSec;
	polar PolMin;
	polar PolHr;

	/* set pointers */{
		PolSec = MakePolar(MaxR/10*8, 6*now.seccond);
		PolMin = MakePolar(MaxR/10*6, 6*now.minute+(PolSec.a/60));
		PolHr =  MakePolar(MaxR/10*4, 30*now.hour+(PolMin.a/12));
		// rotate
		PolSec.a-=90;
		PolMin.a-=90;
		PolHr.a-=90;
	}

	while (true){
		//TODO: write this with fb chars
		/* write time */{
			move(top.y/16-1,mid.x/8-4);
			printf("%8s", " ");
			move(top.y/16-1,mid.x/8-4);
			printf("%02d:%02d:%02d", now.hour, now.minute, now.seccond);
			fflush(stdout);
		}


		/* draw the pointers */ {
			TDrawPolarLine(jar, mid, PolSec, ColSec, 2);
			TDrawPolarLine(jar, mid, PolMin, ColMin, 2);
			TDrawPolarLine(jar, mid, PolHr , ColHr , 2);

			now.seccond++;
			sleep(1);

			TDrawPolarLine(jar, mid, PolSec, white, 2);
			TDrawPolarLine(jar, mid, PolMin, white, 2);
			TDrawPolarLine(jar, mid, PolHr , white, 2);
		}

		/* move the pointers */ {
			PolSec.a+=6;
			PolMin.a+=0.1;
			PolHr.a+=0.0017;
		}

		if (now.seccond == 60) {
			time(&rn);
			now = FmtTime(rn);
			//now.seccond = 0;
			//now.minute++;
			//if (now.minute == 60) {
			//	now.minute = 0;
			//	now.hour++;
			//	if (now.hour == 25) {
			//		now.hour = 1;
			//	}
			//}

			/* reset pointers */{
				// fix any long-term delay
				PolSec = MakePolar(MaxR/10*8, 6*now.seccond);
				PolMin = MakePolar(MaxR/10*6, 6*now.minute+(PolSec.a/60));
				PolHr =  MakePolar(MaxR/10*4, 30*now.hour+(PolMin.a/12));
				// rotate
				PolSec.a-=90;
				PolMin.a-=90;
				PolHr.a-=90;
			}
		}

	}


	CloseFb(jar);
	ShowCursor();
	return 0;
}



