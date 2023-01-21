//funcs

void _P_polar (polar pl) {
	printf("r:%Lf, ang:%Lf\n", pl.r, pl.a);
}

void _P_point (point p) {
	printf("y:%i, x:%i\n", p.y, p.x);
}

void _P_line (line l) {
	printf("%i,%i -> %i,%i\n", l.a.y, l.a.x, l.b.y, l.b.x);
}

void _P_triangle (triangle t) {
	printf("%i,%i; %i,%i; %i,%i\n",
		t.a.y, t.a.x,
		t.b.y, t.b.x,
		t.c.y, t.c.x
	);
}

void _P_quad (quad q) {
	printf("%i,%i; %i,%i; %i,%i; %i,%i\n",
		q.a.y, q.a.x,
		q.b.y, q.b.x,
		q.d.y, q.d.x,
		q.c.y, q.c.x
	);
}

void _P_ppoint (ppoint p) {
	printf("%Lf,%Lf\n", p.y, p.x);
}

void _P_pline (pline l) {
	printf("%Lf,%Lf -> %Lf,%Lf\n", l.a.y, l.a.x, l.b.y, l.b.x);
}

void _P_ptriangle (ptriangle t) {
	printf("%Lf,%Lf; %Lf,%Lf; %Lf,%Lf\n",
		t.a.y, t.a.x,
		t.b.y, t.b.x,
		t.c.y, t.c.x
	);
}

void _P_pquad (pquad q) {
	printf("%Lf,%Lf; %Lf,%Lf; %Lf,%Lf; %Lf,%Lf\n",
		q.a.y, q.a.x,
		q.b.y, q.b.x,
		q.d.y, q.d.x,
		q.c.y, q.c.x
	);
}

//TODO: can't write to the end of the screen
// last 8 lines can't be used

// specific interact
struct fbjar InitFb() {
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
		.rows = (screensize / finfo.line_length),
		.cols = finfo.line_length/(vinfo.bits_per_pixel/8)-10,
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
	fflush(jar.log);
	fclose(jar.log);
	jar.tty = NULL;
	close(jar.fd);
}

void SHandleInt( int sig ) {
	ShowCursor();
	CloseFb(GlobalJar);
	printf("kill sig: %d SIGINT\n", sig);
	printf("killing family\n");
	kill(0, 2);
	exit(129);
}

// easy interact
void StopBuffy(struct fbjar jar) {
	if (GlobalJar.tty){
		CloseFb(GlobalJar);
	}
	if (jar.tty) {
		if (jar.fd == -1) { // malloc, not mmap
			free(jar.fbmem);
		} else {
			CloseFb(jar);
			munmap(jar.fbmem, jar.screensize);
		}
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

// Make
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

ppoint pMakepPoint(const point p) {
	ppoint r = {.y = (lfloat)p.y, .x = (lfloat)p.x};
	return r;
}

point pMakePoint(const ppoint p) {
	point r = {.y = (int)lroundl(p.y), .x = (int)lroundl(p.x)};
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

//TODO: just, please
polar pMakePolar(const point p) {
	polar ret;
	ret.r = GetDistance(p, __BUFFY_H_POINT_ZZ);
	if (!p.y) {
		if (p.y > 0) {
			ret.a = 270;
		} else if (p.y) {
			ret.a = 90;
		} else {
			ret.a = 0;
		}
	} else {
		ret.a = atanl(p.y/p.x)*180.0/PI;
	}
	return ret;
}

polar ppMakePolar(const ppoint p) {
	polar ret;
	ret.a = atanl(p.y/p.x)*(180.0/PI);
	ret.r = pGetDistance(p, __BUFFY_H_POINT_ZZ);
	return ret;
}

triangle MakeTriangle(const point a, const point b, const point c) {
	triangle t = {.a = a, .b = b, .c = c};
	return t;
}

triangle MakeSTriangle(const point a, const point b, const point c) {
	triangle t = {.a = a, .b = b, .c = c};
	SSortTriangle(&t);
	return t;
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

point ppMovePoint(const point p, const ppoint d /*diff*/ ) {
	point r = {.y = p.y+d.y, .x = p.x+d.x};
	return r;
}

ppoint MovepPoint(const ppoint p, const int y, const int x) {
	ppoint r = {.y = p.y+y, .x = p.x+x};
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

polar MovePolar(const polar pl, const int y, const int x) {
	return ppMakePolar(MovepPoint(plMakepPoint(pl), y, x));
}

polar pMovePolar(const polar pl, const point p) {
	return ppMakePolar(pMovepPoint(plMakepPoint(pl), p));
}

polar RotatePolar(polar pl, int ang) {
	pl.a+=ang;
	return pl;
}

//TODO: plMovePolar
//polar plMovePolar(const polar pl) {
//
//}

polar ppMovePolar(const polar pl, const ppoint p) {
	return ppMakePolar(ppMovepPoint(plMakepPoint(pl), p));
}

//TODO: [pl]RotatePolar

line LineSwap(line l) {
	point t = l.b;
	l.b = l.a;
	l.a = t;
	return l;
}

line ExpandLine(line l, lfloat b) {
	l.b = pMakePoint(D1LineLerp(l, 1+b));
	return l;
}

line RevExpandLine(line l, lfloat b) {
	l = LineSwap(l);
	l.b = pMakePoint(D1LineLerp(l, 1+b));
	l = LineSwap(l);
	return l;
}

// compact line (line.b = lerp p < 1)
// move triangle
// rotate triangle (rotate matrix?)
// move quad

point plMakePoint (polar plr) {
	point pnt;
	float a = plr.a*(PI/180);
	pnt.x = plr.r*cos(a);
	pnt.y = plr.r*sin(a);
	return pnt;
}

ppoint plMakepPoint (polar plr) {
	ppoint pnt;
	float a = plr.a*(PI/180);
	pnt.x = plr.r*cos(a);
	pnt.y = plr.r*sin(a);
	return pnt;
}

// top = point closest to 0,0
point GetTriangleTop(triangle tri) {
	point zz = MakePoint(0,0);
	point top = tri.a;

	if (GetDistance(tri.b, zz) < GetDistance(top, zz)) {
		top = tri.b;
	}
	if (GetDistance(tri.c, zz) < GetDistance(top, zz)) {
		top = tri.c;
	}
	return top;
}

// bot = point furthest to 0,0
point GetTriangleBot(triangle tri) {
	point zz = MakePoint(0,0);
	point bot = tri.a;

	if (GetDistance(tri.b, zz) < GetDistance(bot, zz)) {
		bot = tri.b;
	}
	if (GetDistance(tri.c, zz) < GetDistance(bot, zz)) {
		bot = tri.c;
	}
	return bot;
}

point GetTriangleCont(triangle tri) {
	if (!pEq(tri.a, GetTriangleTop(tri)) || !pEq(tri.a, GetTriangleBot(tri))) 
		return tri.a;
	if (!pEq(tri.b, GetTriangleTop(tri)) || !pEq(tri.b, GetTriangleBot(tri))) 
		return tri.b;
	return tri.c;
}

// top, cont, bot
void SortTriangle(triangle tri, point *dest) {
	dest[0] = tri.a;
	dest[1] = tri.b;
	dest[2] = tri.c;
	// dest[0,1] = sort(a, b)
	if (pGt(tri.a, tri.b)) {
		dest[0] = tri.b;
		dest[1] = tri.a;
	}
	// dest[0,2] =  sort(min(a,b), c)
	if (pGt(dest[0], tri.c)) {
		dest[2] = dest[0];
		dest[0] = tri.c;
	}
	// dest[0,1] = sort(dest[0,1])
	if (pGt(dest[0], dest[1])) {
		point swap = dest[0];
		dest[0] = dest[1];
		dest[1] = swap;
	}
	if (pGt(dest[1], dest[2])) {
		point swap = dest[1];
		dest[1] = dest[2];
		dest[2] = swap;
	}
}

void SSortTriangle(triangle *tri) {
	point p[3] = {0};
	SortTriangle(*tri, p);
	vApplyTriangle(tri, p);
}

void vApplyTriangle(triangle *tri, point *source) {
	tri->a = source[0];
	tri->b = source[1];
	tri->c = source[2];
}

point GetLineMid(line l) {
	return pMakePoint(D1LineLerp(l, 0.5));
}

point GetPointsMid(point a, point b) {
	return pMakePoint(D1PointLerp(a, b, 0.5));
}

ppoint pGetLineMid(line l) {
	return D1LineLerp(l, 0.5);
}

ppoint pGetPointsMid(point a, point b) {
	return D1PointLerp(a, b, 0.5);
}

triangle RotateTriangle(triangle tri, int ang) {
	point p[3] = {0};
	SortTriangle(tri, p);
	ppoint mid = pGetPointsMid(
		p[1], // control
		GetPointsMid(
			p[0], // top
			p[2] // bot
		)
	);
	TDrawpPoint(GlobalJar, mid, RGB(255,128,0), 5);
	mid = ppMul(mid, -1);
	tri.a = ppMovePoint(tri.a, mid);
	tri.b = ppMovePoint(tri.b, mid);
	tri.c = ppMovePoint(tri.c, mid);
	mid = ppMul(mid, -1);

	polar a = pMakePolar(tri.a);
	polar b = pMakePolar(tri.b);
	polar c = pMakePolar(tri.c);

	SDrawPolarLine(GlobalJar, pMakePoint(mid), a);
	SDrawPolarLine(GlobalJar, pMakePoint(mid), b);
	SDrawPolarLine(GlobalJar, pMakePoint(mid), c);

	_P_polar(a);
	_P_polar(b);
	_P_polar(c);
	a = RotatePolar(a, ang);
	b = RotatePolar(b, ang);
	c = RotatePolar(c, ang);
	_P_polar(a);
	_P_polar(b);
	_P_polar(c);

	DrawPolarLine(GlobalJar, pMakePoint(mid), a, RGB(128, 128, 128));;
	DrawPolarLine(GlobalJar, pMakePoint(mid), b, RGB(128, 128, 128));
	DrawPolarLine(GlobalJar, pMakePoint(mid), c, RGB(128, 128, 128));

	return MakeTriangle(
		pAddpp(plMakePoint(RotatePolar(pMakePolar(tri.a), ang)), mid),
		pAddpp(plMakePoint(RotatePolar(pMakePolar(tri.b), ang)), mid),
		pAddpp(plMakePoint(RotatePolar(pMakePolar(tri.c), ang)), mid)
	);
}

//TODO: plMakePoint instead of transform name
point SPolarToCoord (polar plr) {
	point pnt;
	float a = plr.a*(PI/180);
	pnt.x = plr.r*cos(a);
	pnt.y = plr.r*sin(a);
	return pnt;
}

ppoint PolarTopCoord (polar plr, point from) {
	ppoint pnt;
	float a = plr.a*(PI/180);
	pnt.y = from.y+plr.r*sin(a);
	pnt.x = from.x+plr.r*cos(a);
	return pnt;
}

// transform
point pMul(point p, int m) {
	p.x = p.x*m;
	p.y = p.y*m;
	return p;
}

ppoint ppMul(ppoint p, int m) {
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

point pAddp(point p, point a) {
	p.x = p.x+a.x;
	p.y = p.y+a.y;
	return p;
}

point pAddpp(point p, ppoint a) {
	p.x = p.x+a.x;
	p.y = p.y+a.y;
	return p;
}

point pSub(point p, int s) {
	p.x = p.x-s;
	p.y = p.y-s;
	return p;
}

point pSubp(point p, point s) {
	p.x = p.x-s.x;
	p.y = p.y-s.y;
	return p;
}

point pSubpp(point p, ppoint s) {
	p.x = p.x-s.x;
	p.y = p.y-s.y;
	return p;
}

int pSum(point p) {
	return p.y+p.x;
}

inline bool pEq(point a, point b) {
	return a.y == b.y && a.x == b.x;
}

inline bool pGt(point a, point b) {
	return GetDistance(a, __BUFFY_H_POINT_ZZ) > GetDistance(b, __BUFFY_H_POINT_ZZ);
}

inline bool pSt(point a, point b) {
	return GetDistance(a, __BUFFY_H_POINT_ZZ) < GetDistance(b, __BUFFY_H_POINT_ZZ);
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

inline ppoint D1pPointLerp(ppoint a, ppoint b, lfloat t) { // t as in time
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
	return sqrt( isquare(a.y-b.y) + isquare(a.x-b.x));
}

inline lfloat pGetDistance(const ppoint a, const point b) {
	return sqrtl( powl(a.y-b.y, 2) + powl(a.x-b.x, 2));
}

inline lfloat ppGetDistance(const ppoint a, const ppoint b) {
	return sqrtl( powl(a.y-b.y, 2) + powl(a.x-b.x, 2));
}

inline float GetADistance(const point a, const point b) {
	return (
		abs(a.y-b.y)+ abs(a.x-b.x)
	);
}

inline point GetpDistance(const point a, const point b) {
	return MakePoint(
		a.y-b.y,
		a.x-b.x
	);
}

inline point GetpADistance(const point a, const point b) {
	return MakePoint(
		abs(a.y-b.y),
		abs(a.x-b.x)
	);
}

//TODO: GetAngle(p ref, p check) -> lfloat
lfloat GetAngle(point reference, const point check) {
	reference = pMul(reference, -1);
	// invert reference
	ppoint ato = pMakepPoint(pAddp(check, reference));
	// distance to check from old reference = distance to check from 0,0
	//if (!ato.x) {
	//	return
	//}
	return atanl(ato.x/ato.y);
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

bool CheckpPIJ (struct fbjar jar, ppoint p) {
	if ((uint)lroundl(p.y) >= jar.rows || (uint)lroundl(p.x) >= jar.cols) {
		fprintf( jar.log,
			"y:%.2Lf max: %i, x:%.2Lf max: %i\n",
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

// if in release mode
// LoadBytemap will return 0 if OK
// -1 if heigth can't be stored in ssize_t
// -2 if width can't be stored in ssize_t
int LoadBytemap(char *pathname, bytemap *b) {
	FILE *rd = fopen(pathname, "r");

	// size of width, width array
	uint szh = 0, szw = 0;
	ssize_t w = 0, h = 0;
	{
		szh = (uint)getc(rd);
#ifndef RELEASE
		assert(szh <= (uint)sizeof(ssize_t));
#else
		if (szh > (uint)sizeof(ssize_t)) {return -1;}
#endif
		// decresent
		byte ha[szh];
		for (uint i = 0; i<szh; i++)  {
			ha[i] = (byte)getc(rd);
		}


		szw = (uint)getc(rd);

#ifndef RELEASE
			assert(szw <= (uint)sizeof(ssize_t));
#else
			if (szw > (uint)sizeof(ssize_t)) {return -2;}
#endif

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

	b->cont = cont;
	b->width = w;
	b->heigth = h;

	return 0;
}
