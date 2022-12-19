#ifndef BUFFY
#define BUFFY

// cUtil https://github.com/owseiwastaken/cutil
#include "cutil.h"
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

// written char heigth in pixels
#define CHARLEN 16
// written char width in pixels
#define CHARLINELEN 8

#define CHAN_R 0b100
#define CHAN_G 0b010
#define CHAN_B 0b001


// complex defs
struct fbjar {
	uint8* fbmem;
	int bpp; // bytes per pixel
	int xoff, yoff;
	int skip; // skip from y to y+1 (in bytes)
	int rows, cols;
	long int screensize;
	int fd;
	FILE* log;
	char* tty;
};

typedef struct {
	float a, r;
} polar;

// one byte for 8 pixels
// one bit for each
// only control is on/off
typedef struct {
	bool* cont;
	int heigth, width;
} bitmap;

// one byte for pixel
// but i'm thinking of
//  only using 4 or
//  2 bits for each pixel
//  and give up some intensity control
// full control intensity, for now
typedef struct {
	byte* cont;
	int heigth, width;
} bytemap;

// bpp = 3, RGB
typedef struct {
	color* cont;
	int heigth, width;
} bytesmap;

// global jar
struct fbjar GlobalJar;

#include "vars.c"


// include drawers
// func naming [(S)imple][(T)hick,(K)eep,(R)elative](Draw,Apply,Fill)
// var naming
// points
// top = < bot
// bot = > top
// len = bot-top (for (R)elative funcs)
#include "draw/simple.c"
#include "draw/normal.c"
#include "draw/complex.c"
#include "draw/chars.c"

#endif

