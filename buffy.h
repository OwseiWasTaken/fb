#ifndef BUFFY
#define BUFFY

// frame buffer header
#include <linux/fb.h>

// cUtil https://github.com/owseiwastaken/cutil
#include "cutil.h"
// fb
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
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

#define CHAN_R 4 // 0b100
#define CHAN_G 2 // 0b010
#define CHAN_B 1 // 0b001


// complex defs
struct fbjar {
	uint8* fbmem;
	uint bpp; // bytes per pixel
	uint xoff, yoff;
	uint skip; // skip from y to y+1 (in bytes)
	uint rows, cols;
	unsigned long int screensize;
	int fd;
	FILE* log;
	char* tty;
};

// one byte for 8 pixels
// one bit for each
// only control is on/off
typedef struct {
	bool* cont;
	uint heigth, width;
} bitmap;

// one byte for pixel
// but i'm thinking of
//	only using 4 or
//	2 bits for each pixel
//	and give up some intensity control
// full control intensity, for now
typedef struct {
	byte* cont;
	uint heigth, width;
} bytemap;

// bpp = 3, RGB
typedef struct {
	color* cont;
	uint heigth, width;
} bytesmap;

typedef struct {
	point a, b;
} line;

typedef struct {
	ppoint a, b;
} pline;

// sorted triangle = {a = top, b = control & c = bot}
// use SortTriangle(triangle tri, point *dest)
// to make dest[] = top,cont,bot
typedef struct {
	point a, b, c;
} triangle;

typedef struct {
	ppoint a, b, c;
} ptriangle;

typedef struct {
	point a, b, c, d;
} quad/*rilateral*/;

typedef struct {
	ppoint a, b, c, d;
} pquad/*rilateral*/;

// global jar
struct fbjar GlobalJar;
point __BUFFY_H_POINT_ZZ = {.y=0,.x=0};

#include "protos.c" // prototypes for vars.c
#include "draw/protos.c" // prototypes for draw/*
#include "vars.c"

// func naming [(q)uick][(S)imple][(T)hick,(K)eep,(R)elative](Draw,Apply,Fill)
// var naming
// points
// top = < bot
// bot = > top
// len = bot-top (for (R)elative funcs)
// cont = not bot nor top

// include drawers
#include "draw/simple.c"
#include "draw/normal.c"
#include "draw/complex.c"
#include "draw/chars.c"

#endif // BUFFY
