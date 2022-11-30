#ifndef BUFFY
#define BUFFY

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


// simple defs
typedef unsigned int uint;
typedef unsigned char uint8;
typedef char int8;

#define PI 3.141592
// written char heigth in pixels
#define CHARLEN 16
// written char width in pixels
#define CHARLINELEN 8


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
	uint8 R, G, B;
} color;

typedef struct {
	int y, x;
} point;

typedef struct {
	float y, x;
} ppoint;

typedef struct {
	float a;
	float r;
} polar;

typedef struct {
	int year, day, hour, minute, seccond;
	char weekday[4];
	char month[4];
} fmttime;

// one byte for 8 pixels
// one bit for each
// only control is on/off
typedef struct {
	uint8* cont;
	int heigth, width;
} bitmap;

// one byte for pixel
// but i'm thinking of
//  only using 4 or
//  2 bits for each pixel
//  and give up some intensity control
// full control intensity, for now
typedef struct {
	uint8* cont;
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
#include "draw/simple.c"
#include "draw/normal.c"
#include "draw/complex.c"
#include "draw/chars.c"

#endif

//TODO: use memset for clearing and fillrect

