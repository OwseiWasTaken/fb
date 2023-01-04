//funcs

void _P_point (point p) {
	printf("y:%i, x:%i\n", p.y, p.x);
}

//TODO: can't write to the end of the screen
// last 8 lines can't be used

// specific interact
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
		perror("InitFb: cannot open framebuffer device");
		exit(1);
	}
	//printf("The framebuffer device was opened successfully.\n");
	// read  fixed screen information
	if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
		perror("InitFb: can't read fixed information");
		exit(2);
	}
	//  variable screen information
	if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
		perror("InitFb: can't read variable information");
		exit(3);
	}
	//printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);
	// Figure out the size of the screen in bytes
	screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
	// Map the device to memory
	fbp = (uint8 *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	if ( *fbp == (uint8)-1) {
		perror("InitFb: failed to map framebuffer device to memory");
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
		.rows = screensize / finfo.line_length,
		.cols = finfo.line_length/(vinfo.bits_per_pixel/8),
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
	// read fixed screen information
	if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
		perror("Error reading fixed information");
		exit(2);
	}
	// read  variable screen information
	if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
		perror("Error reading variable information");
		exit(3);
	}
	//printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);
	// Figure out the size of the screen in bytes
	screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
	// Map the device to memory
	fbp = malloc(screensize);

	close(fbfd);
	struct fbjar jar = {
		.fd = -1,
		.fbmem = fbp,
		.bpp = vinfo.bits_per_pixel/8,
		.xoff = vinfo.xoffset*vinfo.bits_per_pixel/8,
		.yoff = vinfo.yoffset*vinfo.bits_per_pixel/8,
		.skip = finfo.line_length,// from y to y+1
		.screensize = screensize,
		.rows = (screensize/finfo.line_length)-1,
		.cols = (finfo.line_length/(vinfo.bits_per_pixel/8))-10,
		.tty = ttyname(STDIN_FILENO),
		.log = fopen("/tmp/membuffy.log", "w")
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

void SHandleInt( int sig ) {
	ShowCursor();
	CloseFb(GlobalJar);
	printf("kill sig: %d SIGINT\n", sig);
	exit(129);
}

// easy interact
void StopBuffy(struct fbjar jar) {
	if (GlobalJar.tty != NULL){
		CloseFb(GlobalJar);
	} else {
		CloseFb(jar);
	}
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

// make
point MakePoint(const int y, const int x) {
	point p = {.y = y, .x = x};
	return p;
}

upoint MakeuPoint(const uint y, const uint x) {
	upoint p = {.y = y, .x = x};
	return p;
}

ppoint MakepPoint(const lfloat y, const lfloat x) {
	ppoint p = {.y = y, .x = x};
	return p;
}

point pMakePoint(const ppoint p) {
	point r = {.y = (int)p.y, .x = (int)p.x};
	return r;
}

line MakeLine(const point a, const point b) {
	line r = {.a = a, .b = b};
	return r;
}

pline MakepLine(const ppoint a, const ppoint b) {
	pline r = {.a = a, .b = b};
	return r;
}

line pMakeLine(const ppoint a, const ppoint b) {
	line r = {.a = pMakePoint(a), .b = pMakePoint(b)};
	return r;
}

//pline lMakepLine(const pline l) {
//	pline r = {.a = pMakePoint(l.a), .b = pMakePoint(l.b)};
//	return r;
//}

polar MakePolar(const float r, const float a) {
	polar p = {.r = r, .a = a};
	return p;
}

// move
point MovePoint(const point p, const int y, const int x) {
	point r = {.y = p.y+y, .x = p.x+x};
	return r;
}

point pMovePoint(const point p, const point d /*diff*/ ) {
	point r = {.y = p.y+d.y, .x = p.x+d.x};
	return r;
}

ppoint pMovepPoint(const ppoint p, const point d /*diff*/ ) {
	ppoint r = {.y = p.y+(lfloat)d.y, .x = p.x+(lfloat)d.x};
	return r;
}

ppoint ppMovepPoint(const ppoint p, const ppoint d /*diff*/ ) {
	ppoint r = {.y = p.y+d.y, .x = p.x+d.x};
	return r;
}

// transform
point pMul(point p, int m) {
	p.x = p.x*m;
	p.y = p.y*m;
	return p;
}

point pDiv(point p, int d) {
	p.x = p.x/d;
	p.y = p.y/d;
	return p;
}

point pAdd(point p, int a) {
	p.x = p.x+a;
	p.y = p.y+a;
	return p;
}

point pSub(point p, int s) {
	p.x = p.x-s;
	p.y = p.y-s;
	return p;
}

ppoint PolarToCoord (polar plr) {
	ppoint pnt;
	lfloat a = plr.a*(PI/180);
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

// lfloat = more precision than float
// distance from path a -> b, @ p% walked
inline lfloat lerp(lfloat a, lfloat b, lfloat p) { // p = 0..1
	return a * (1-p) + (b*p);
}

// get

inline ppoint D1PointLerp(point a, point b, lfloat t) { // t as in time
	return MakepPoint(
		lerp(a.y, b.y, t),
		lerp(a.x, b.x, t)
	);
}

inline ppoint D1LineLerp(line l, lfloat t) { // t as in time
	return MakepPoint(
		lerp(l.a.y, l.b.y, t),
		lerp(l.a.x, l.b.x, t)
	);
}

inline ppoint D1pLineLerp(pline l, lfloat t) { // t as in time
	return MakepPoint(
		lerp(l.a.y, l.b.y, t),
		lerp(l.a.x, l.b.x, t)
	);
}

inline float GetDistance(const point a, const point b) {
	return sqrt(
		isquare(a.y-b.y)
		+
		isquare(a.x-b.x)
	);
}

inline point GetpDistance(const point a, const point b) {
	return MakePoint(
		abs(a.y-b.y),
		abs(a.x-b.x)
	);
}

// can also be used to expand a line (if seg > 100)
inline point GetPILSeg (line l, float seg) {
	point dist = GetpDistance(l.a, l.b);
	long double y = (float)dist.y / 100.0 * seg;
	long double x = (float)dist.x / 100.0 * seg;
	return MakePoint((int)y+l.a.y, (int)x+l.a.x);
}

inline point GetRayCast(const line l, const int expand) {
	return GetPILSeg(l, (float)expand+100);
}

inline line RayCast(line l, const int expand) {
	l.b = GetPILSeg(l, (float)expand+100);
	return l;
}

long int GetPixelPos ( struct fbjar jar, int y, int x ) {
	return (y+jar.yoff)*jar.skip + (jar.xoff+x)*jar.bpp;
}

long int GetPixelPnt ( struct fbjar jar, point p ) {
	return (p.y+jar.yoff)*jar.skip + (jar.xoff+p.x)*jar.bpp;
}

uint8* GetFbPnt ( struct fbjar jar, point p ) {
	return jar.fbmem + (p.y+jar.yoff)*jar.skip + (jar.xoff+p.x)*jar.bpp;
}

uint8* GetFbPos ( struct fbjar jar, int y, int x ) {
	return jar.fbmem + (y+jar.yoff)*jar.skip + (jar.xoff+x)*jar.bpp;
}

// cheks
bool PInP(point top, point bot, point check) {
	return (
		top.y < check.y &&
		bot.y > check.y &&
		top.x < check.x &&
		bot.x > check.x
	);
}


bool CheckPIJ (struct fbjar jar, point p) {
	if ((uint)p.y >= jar.rows || (uint)p.x >= jar.cols) {
		fprintf( jar.log,
			"y:%d max: %d, x:%d max: %d\n",
			p.y, jar.rows, p.x, jar.cols);
		fflush(jar.log);
		return false;
	}
	return true;
}

// uP for upoint
bool CheckuPIJ (struct fbjar jar, upoint p) {
	if (p.y >= jar.rows || p.x >= jar.cols) {
		fprintf( jar.log,
			"y:%d max: %d, x:%d max: %d\n",
			p.y, jar.rows, p.x, jar.cols);
		fflush(jar.log);
		return false;
	}
	return true;
}

// ^q for quick
bool qCheckPIJ (struct fbjar jar, point p) {
	return !((uint)p.y >= jar.rows || (uint)p.x >= jar.cols);
}

bool qCheckuPIJ (struct fbjar jar, upoint p) {
	return !(p.y >= jar.rows || p.x >= jar.cols);
}

bool CheckInJar (struct fbjar jar, uint y, uint x) {
	if (y >= jar.rows || x >= jar.cols) {
		fprintf( jar.log,
			"y:%d max: %d, x:%d max: %d\n",
			y, jar.rows, x, jar.cols);
		fflush(jar.log);
		return false;
	}
	return true;
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

bytemap LoadBytemap(char *pathname) {
	FILE *rd = fopen(pathname, "r");

	// size of width, width array
	uint szh = 0, szw = 0;
	ssize_t w = 0, h = 0;
	{
		szh = (uint)getc(rd);
		assert(szh < (uint)sizeof(ssize_t));
		// decresent
		byte ha[szh];
		for (uint i = 0; i<szh; i++)  {
			ha[i] = (byte)getc(rd);
		}


		szw = (uint)getc(rd);
		assert(szw < (uint)sizeof(ssize_t));
		// decresent
		byte wa[szw];
		for (uint i = 0; i<szw; i++)  {
			wa[i] = (byte)getc(rd);
		}

		//byte cont[w*h];
		//read(rfd, cont, w*h);

		//TODO test this
		for (uint i = 0; i<szh; i++) {
			h = 255*h+ha[i];
		}
		for (uint i = 0; i<szw; i++) {
			w = 255*w+wa[i];
		}
	}
	byte cont[h*w];
	for (long i = 0; i<h*w; i++) {cont[i] = (byte)getc(rd);}
	//read(rfd, cont, h*w);
	fclose(rd);

	bytemap b;
	b.cont = cont;
	b.width = w;
	b.heigth = h;

	return b;
}
