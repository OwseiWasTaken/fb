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

// don't open /dev/fb, just malloc
struct fbjar InitMemJar () {
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
	fbp = malloc(screensize);
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
	return jar;
}

void CloseFb(struct fbjar jar) {
	munmap(jar.fbmem, jar.screensize);
	fflush(jar.log);
	fclose(jar.log);
	jar.tty = NULL;
	close(jar.fd);
}

void StopBuffy(struct fbjar jar) {
	if (GlobalJar.tty != NULL){
		CloseFb(GlobalJar);
	} else {
		CloseFb(jar);
	}
}

point MakePoint(int y, int x) {
	point p = {.y = y, .x = x};
	return p;
}

polar MakePolar(float r, float a) {
	polar p = {.r = r, .a = a};
	return p;
}

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

bool CheckInJar (struct fbjar jar, int y, int x) {
	if (y >= jar.rows || y < 0 || x >= jar.cols || x < 0) {
		fprintf( jar.log,
			"y:%d max: %d, x:%d max: %d\n",
			y, jar.rows, x, jar.cols);
		fflush(jar.log);
		return false;
	}
	return true;
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

void SHandleInt( int sig ) {
	ShowCursor();
	CloseFb(GlobalJar);
	exit(129);
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

bool PInP(point top, point bot, point check) {
	return (
		top.y < check.y &&
		bot.y > check.y &&
		top.x < check.x &&
		bot.x > check.x
	);
}

void UpdateBuffer(struct fbjar jar, uint8* newbuff) {
	memcpy(jar.fbmem, newbuff, jar.screensize);
}

void UpdateJar(struct fbjar * restrict jar, struct fbjar * restrict newjar) {
	memcpy(jar->fbmem, newjar->fbmem, jar->screensize);
}

bool* BytesToCont(int heigth, int width, byte* Bcont) {
	int size = heigth*width;
	bool *bcont = malloc(size);
	for (int i = 0; i<size/8; i++) {
		for (int j = 0; j<8; j++ ) {
			// i = array->byte offset, j = byte->bit offset
			bcont[i*8+j] = (Bcont[i]&(1<<(7-j)))!=0;
		}
	}
	return bcont;
}

void PutBytesInBitmap(bitmap *b, byte* Bcont) {
	int size = b->heigth*b->width;
	bool *bcont = malloc(size);
	for (int i = 0; i<size/8; i++) {
		for (int j = 0; j<8; j++ ) {
			// i = array->byte offset, j = byte->bit offset
			bcont[i*8+j] = (Bcont[i]&(1<<(7-j)))!=0;
		}
	}
	b->cont = bcont;
}

uint8* ReadByteFb(struct fbjar jar, point top, point bot) {
	uint8 *location = GetFbPnt(jar, top);
	int limy = top.y - bot.y;
	int limx = top.x - bot.x;
	uint8* cont = malloc(limy*limx);
	for (int i = 0; i<limy; i++) {
		for (int j = 0; j<limx; j++) {
			cont[i*limx+j] = (location[0]+location[1]+location[2])/3;
			location += jar.bpp;
		}
		// next line
		location -= limx*jar.bpp;
		location += jar.skip;
	}
	return cont;
}

color* ReadBytesFb(struct fbjar jar, point top, point bot) {
	uint8 *location = GetFbPnt(jar, top);
	int limy = top.y - bot.y;
	int limx = top.x - bot.x;
	color* cont = malloc(limy*limx*sizeof(color));
	for (int i = 0; i<limy; i++) {
		for (int j = 0; j<limx; j++) {
			cont[i*limx+j] = RGB(location[2],location[1],location[0]);
			location += jar.bpp;
		}
		// next line
		location -= limx*jar.bpp;
		location += jar.skip;
	}
	return cont;
}

void ReadByteFbIntoBytemap(struct fbjar jar, point top, bytemap *b) {
	uint8 *location = GetFbPnt(jar, top);
	int limy = b->heigth;
	int limx = b->width;
	uint8* cont = malloc(limy*limx);
	for (int i = 0; i<limy; i++) {
		for (int j = 0; j<limx; j++) {
			cont[i*limx+j] = (location[0]+location[1]+location[2])/3;
			location += jar.bpp;
		}
		// next line
		location -= limx*jar.bpp;
		location += jar.skip;
	}
	b->cont = cont;
}

void ReadByteFbIntoBytesmap(struct fbjar jar, point top, bytesmap *b) {
	uint8 *location = GetFbPnt(jar, top);
	int limy = b->heigth;
	int limx = b->width;
	color* cont = malloc(limy*limx*sizeof(color));
	for (int i = 0; i<limy; i++) {
		for (int j = 0; j<limx; j++) {
			cont[i*limx+j] = RGB(location[2],location[1],location[0]);
			location += jar.bpp;
		}
		// next line
		location -= limx*jar.bpp;
		location += jar.skip;
	}
	b->cont = cont;
}

