//funcs

//TODO: can't write to the end of the screen
// last 8 lines can't be used

// tbh no idea how this works
// but it works
struct fbjar InitFb()
{
	int fbfd = 0;
	uint8 *fbp = 0;
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
	fbp = (uint8 *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
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
		.tty = ttyname(STDIN_FILENO),
		.log = fopen("/tmp/buffy.log", "w")
	};
	// GlobalJar for Sig handleing etc
	GlobalJar = jar;
	return jar;
}

void CloseFb(struct fbjar jar) {
	munmap(jar.fbmem, jar.screensize);
	fflush(jar.log);
	fclose(jar.log);
	close(jar.fd);
}

void InitTerm ( struct fbjar jar ) {
	struct termios term;
	tcgetattr(STDIN_FILENO, &term);

	char reconfig[36+10]; /* 36 = comamnd len, 10 = /dev/ttyNN */

	fprintf(jar.log, "this tty: %s\n", jar.tty);
	system("/bin/stty -g > /tmp/restore");
	fprintf(jar.log, "tty config saved to '/tmp/restore'\n");

	sprintf(reconfig, "/bin/stty -F %s -echo cbreak min 1", jar.tty);
	system(reconfig);
}

void CloseTerm (struct fbjar jar) {
	system("/bin/stty $(/bin/cat /tmp/restore)");
	fprintf(jar.log, "\ntty config restored\n");
	system("/bin/rm /tmp/restore");
	fprintf(jar.log, "file '/tmp/restore' deleted\n");
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

polar MakePolar(float r, float a) {
	polar p = {.r = r, .a = a};
	return p;
}

// ANSII (text) rgb
char* TRGB(uint8 R, uint8 G, uint8 B) {
	char* buff = malloc(21);
	sprintf(buff, "\x1b[38;2;%d;%d;%dm", R, G, B);
	return buff;
}

color RGB(uint8 R, uint8 G, uint8 B) {
	color c = {.R = R, .G = G, .B = B};
	return c;
}

//TODO: do this, but no segfault
//char* TRGB(char r, char g, char b) {
//	char* ret = "                  ";
//	sprintf(ret, "\x1b[38;2;%c;%c;%cm", r, g, b);
//	return ret;
//}

long int GetPixelPos ( struct fbjar jar, int y, int x ) {
	return (y+jar.yoff)*jar.skip + (jar.xoff+x)*jar.bpp;
}

long int GetPixelPnt ( struct fbjar jar, point p ) {
	return (p.y+jar.yoff)*jar.skip + (jar.xoff+p.x)*jar.bpp;
}

uint8* GetFbPnt ( struct fbjar jar, point p ) {
	return jar.fbmem+((p.y+jar.yoff)*jar.skip + (jar.xoff+p.x)*jar.bpp);
}

uint8* GetFbPos ( struct fbjar jar, int y, int x ) {
	return jar.fbmem+((y+jar.yoff)*jar.skip + (jar.xoff+x)*jar.bpp);
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

int ipow(int base, int power) {
	return (int)pow((double)base, (double)power);
}

int isquare (int base) {
	return ipow(base, 2);
}

ppoint PolarToCoord (polar plr) {
	ppoint pnt;
	float a = plr.a*(PI/180);
	pnt.x = plr.r*cos(a);
	pnt.y = plr.r*sin(a);
	return pnt;
}

point SPolarToCoord (polar plr) {
	point pnt;
	float a = plr.a*(PI/180);
	pnt.x = plr.r*cos(a);
	pnt.y = plr.r*sin(a);
	return pnt;
}

fmttime FmtTime(time_t rn, int UTF) {
	fmttime now;

	char* timetext = ctime(&rn);
	now.weekday[0] = timetext[0];
	now.weekday[1] = timetext[1];
	now.weekday[2] = timetext[2];
	now.weekday[4] = '\0';

	now.month[0] = timetext[4];
	now.month[1] = timetext[5];
	now.month[2] = timetext[6];
	now.month[3] = '\0';

	now.minute = (rn%(60*60))/60;
	now.seccond= rn%60;
	now.year = 1970+(int)(rn/(24.0*3600.0)/365.25);
	now.hour = rn/3600%24+(UTF);
	if (now.hour < 1) {
		now.hour = 24+now.hour;
	}

	now.day = timetext[9]-'0';
	if (timetext[8] != ' ') {
		now.day += 10*(timetext[8]-'0');
	};
	return now;
}

char* FmtTimeToString(fmttime now) {
	char* buff = malloc(38);
	sprintf(buff, "s:%d\nm:%d\nh:%d\nd:%d\ny:%d\n\nM:%s\nW:%s",
		now.seccond, now.minute, now.hour, now.day, now.year, now.month, now.weekday);
	return buff;
}

void SHandleInt( int sig ) {
	ShowCursor();
	CloseFb(GlobalJar);
	exit(0);
}

struct fbjar InitBuffy() {
	struct sigaction act;
	act.sa_handler = SHandleInt;
	sigaction(SIGINT, &act, NULL);
	struct fbjar jar = InitFb();
	fprintf(jar.log, "fb size: line cols:%d, rows: %d\n", jar.cols-1, jar.rows-1);
	fprintf(jar.log, "tty: %s\n", jar.tty);
	//uint8* chars = ReadChars(1);
	//fprintf(jar.log, "chars loaded from 'draw/font'\n");
	return jar;
}

