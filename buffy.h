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
//TODO: dynamic
#define UTFDIFF -3

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

// global jar
struct fbjar GlobalJar;

#include "vars.c"


// include drawers
#include "draw/simple.c"
#include "draw/normal.c"
#include "draw/complex.c"
#include "draw/chars.c"

#endif
