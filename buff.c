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


int main ( int argc, char** argv ) {
	// init
	signal(SIGINT, SHandleInt);
	struct fbjar jar = InitFb();
	fprintf(jar.log, "fb size: line cols:%d, rows: %d\n", jar.cols-1, jar.rows-1);
	fprintf(jar.log, "tty: %s\n", jar.tty);
	//uint8* chars = ReadChars(1);
	//fprintf(jar.log, "chars loaded from 'draw/font'\n");

	bool debug = false;
	bool digital = false;
	int MaxR = 250;
	int y = jar.rows/2-MaxR;
	int x = jar.cols/2-MaxR;
	/* parse args */{
		for (int i = 0 ; i<argc ; i++) {
			if (argv[i][0] == '-') {
				if (!strcmp(argv[i], "--debug")) {
					debug = true;
				} else if (!strcmp(argv[i], "--digital")) {
					digital = true;
				} else if (!strcmp(argv[i], "-d")) {
					digital = true;
				} else if (!strcmp(argv[i], "-r")) {
					i++;
					assert(i < argc); // read radius size
					MaxR = atoi(argv[i]);
					if (MaxR == 0 && !(argv[i][0] == '0')) {
						assert(0);//can't parse -r input
					}
				} else if (!strcmp(argv[i], "-y")) {
					i++;
					assert(i < argc); // read radius size
					y = atoi(argv[i]);
					if (y == 0 && !(argv[i][0] == '0')) {
						assert(0);//can't parse -r input
					}
				} else if (!strcmp(argv[i], "-x")) {
					i++;
					assert(i < argc); // read radius size
					x = atoi(argv[i]);
					if (x == 0 && !(argv[i][0] == '0')) {
						assert(0);//can't parse -r input
					}
				}
			}
		}
	}
	HideCursor();
	x+=MaxR;
	y+=MaxR;

	time_t rn;
	time(&rn);

	fmttime now = FmtTime(rn);

	color white = RGB(255,255,255);
	color ColSec = RGB(255,0,0);
	color ColMin = RGB(255,0,255);
	color ColHr = RGB(0,0,0);

	// max pointer len
	point top = MakePoint(y-MaxR,x-MaxR);
	point mid = MakePoint(y,x);

	SFillCircle(jar, top, MaxR);

	polar PolSec, PolMin, PolHr;

	/* set pointers */ {
		PolSec = MakePolar(MaxR/10*8, 6*now.seccond);
		PolMin = MakePolar(MaxR/10*6, 6*now.minute+(now.seccond/10));
		PolHr =  MakePolar(MaxR/10*4, 30*now.hour+(now.minute/2));
		// rotate
		PolSec.a-=90;
		PolMin.a-=90;
		PolHr.a-=90;
	}

	while (1){
		//TODO: write this with fb chars
		SFillCircle(jar, top, MaxR);
		if (digital) /* write time */{
			move(top.y/16-1,mid.x/8-4);
			printf("%8s", " ");
			move(top.y/16-1,mid.x/8-4);
			printf("%02d:%02d:%02d", now.hour, now.minute, now.seccond);
			fflush(stdout);
		}

		/* draw the pointers */ {
			TDrawPolarLine(jar, mid, PolSec, ColSec, 2);
			TDrawPolarLine(jar, mid, PolMin, ColMin, 3);
			TDrawPolarLine(jar, mid, PolHr , ColHr , 4);

			now.seccond++;
			sleep(1);

			TDrawPolarLine(jar, mid, PolSec, white, 2);
			TDrawPolarLine(jar, mid, PolMin, white, 3);
			TDrawPolarLine(jar, mid, PolHr , white, 4);
		}

		/* move the pointers */ {
			PolSec.a+=6;
			PolMin.a+=0.1;
			PolHr.a+=0.0017;
		}

		if (now.seccond == 60) {
			if (debug) {
				now.seccond = 0;
				now.minute++;
				if (now.minute == 60) {
					now.minute = 0;
					now.hour++;
					if (now.hour == 25) {
						now.hour = 1;
					}
				}
			} else {
				time(&rn);
				now = FmtTime(rn);
			}

			/* reset pointers */{
				// fix any long-term delay
				PolSec = MakePolar(MaxR/10*8, 6*now.seccond);
				PolMin = MakePolar(MaxR/10*6, 6*now.minute+(now.seccond/10));
				PolHr =  MakePolar(MaxR/10*4, 30*now.hour+(now.minute/2));
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



