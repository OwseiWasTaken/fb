// simple defs
typedef unsigned int uint;
typedef unsigned char uint8;
typedef char int8;

// complex defs
struct fbjar {
	char* fbmem;
	int bpp; // bytes per pixel
	int xoff, yoff;
	int skip; // skip from y to y+1 (in bytes)
	int rows, cols;
	long int screensize;
	int fd;
	FILE* log;
	char* tty;
};

struct Color {
	uint8 R, G, B;
};

struct Point {
	int y, x;
};

typedef struct Color color;
typedef struct Point point;

//funcs

// tbh no idea how this works
// but it works
struct fbjar InitFb()
{
	int fbfd = 0;
	char *fbp = 0;
	long int screensize = 0;
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
	// Open the file for reading and writing
	fbfd = open("/dev/fb0", O_RDWR);
	if (fbfd == -1) {
		perror("Error: cannot open framebuffer device");
		exit(1);
	}
	//printf("The framebuffer device was opened successfully.\n");
	// Get fixed screen information
	if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
		perror("Error reading fixed information");
		exit(2);
	}
	// Get variable screen information
	if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
		perror("Error reading variable information");
		exit(3);
	}
	//printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);
	// Figure out the size of the screen in bytes
	screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
	// Map the device to memory
	fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	if ( *fbp == -1) {
		perror("Error: failed to map framebuffer device to memory");
		exit(4);
	}
	
	struct fbjar jar = {
		.fd = fbfd,
		.fbmem = fbp,
		.bpp = vinfo.bits_per_pixel/8,
		.xoff = vinfo.xoffset*vinfo.bits_per_pixel/8,
		.yoff = vinfo.yoffset*vinfo.bits_per_pixel/8,
		.skip = finfo.line_length,// from y to y+1
		.screensize = screensize,
		.rows = (screensize/finfo.line_length)-1,
		.cols = (finfo.line_length/(vinfo.bits_per_pixel/8))-10,
		.log = fopen("log.txt", "w"),
		.tty = ttyname(STDIN_FILENO)
	};
	return jar;
}

void InitTerm ( struct fbjar jar ) {
	struct termios term;
	tcgetattr(STDIN_FILENO, &term);

	char reconfig[36+10]; /* 36 = comamnd len, 10 = /dev/ttyNN */

	fprintf(jar.log, "this tty: %s\n", jar.tty);
	system("/bin/stty -g > restore");
	fprintf(jar.log, "tty config saved to 'restore'\n");

	sprintf(reconfig, "/bin/stty -F %s -echo cbreak min 1", jar.tty);
	system(reconfig);
}

void CloseTerm (struct fbjar jar) {
	system("/bin/stty $(/bin/cat restore)");
	fprintf(jar.log, "\ntty config restored\n");
	system("/bin/rm restore");
	fprintf(jar.log, "file 'restore' deleted\n");
}

void CloseFb(struct fbjar jar) {
	munmap(jar.fbmem, jar.screensize);
	fflush(jar.log);
	fclose(jar.log);
	close(jar.fd);
}

void move(int y, int x) {
	printf("\x1B[%d;%dH", y+1, x+1);
}

void HideCursor() {
	puts("\x1b[?25l");
}

void ShowCursor() {
	puts("\x1b[?25h");
}

point GetTerminalSize() {
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	point p = {.y = w.ws_col, .x = w.ws_row};
	return p;
}


point MakePoint(int y, int x) {
	point p = {.y = y, .x = x};
	return p;
}

color RGB(char R, char G, char B) {
	color c = {.R = R, .G = G, .B = B};
	return c;
}

//TODO: do this, but no segfault
//char* TRGB(char r, char g, char b) {
//	char* ret = "                  ";
//	sprintf(ret, "\x1b[38;2;%c;%c;%cm", r, g, b);
//	return ret;
//}

inline long int GetPixelPos ( struct fbjar jar, int y, int x ) {
	return (y+jar.yoff)*jar.skip + (jar.xoff+x)*jar.bpp;
}

bool CheckPIJ (struct fbjar jar, point p) {
	if (p.y >= jar.rows || p.y < 0 || p.x >= jar.cols || p.x < 0) {
		fprintf( jar.log,
			"y:%d max: %d, x:%d max: %d\n",
			p.y, jar.rows, p.x, jar.cols);
		fflush(jar.log);
		return false;
	}
	return true;
}

bool CheckInjar (struct fbjar jar, int y, int x) {
	if (y >= jar.rows || y < 0 || x >= jar.cols || x < 0) {
		fprintf( jar.log,
			"y:%d max: %d, x:%d max: %d\n",
			y, jar.rows, x, jar.cols);
		fflush(jar.log);
		return false;
	}
	return true;
}

