void DrawPartLine (
	struct fbjar jar,
	int y, int StartX, int EndX,
	color RGB
) {
	assert(CheckInjar(jar, y, StartX));
	assert(CheckInjar(jar, y, EndX));

	uint8* location = GetFbPos(jar, y, StartX);
	int Xlen = (EndX - StartX)*jar.bpp;
	int x;

	for (x = 0 ; x<Xlen ; x+=jar.bpp) {
		*(location + 0 + x) = RGB.B;
		*(location + 1 + x) = RGB.G;
		*(location + 2 + x) = RGB.R;
	}
}

void DrawPartCollum (
	struct fbjar jar,
	int x, int StartY, int EndY,
	color RGB
) {
	assert(CheckInjar(jar, StartY, x));
	assert(CheckInjar(jar, EndY, x));

	uint8* location = GetFbPos(jar, StartY, x);
	int Ylen = (EndY - StartY);
	int sk = jar.yoff*jar.skip;

	for (int y = 0;y<Ylen;y++) {
		sk+=jar.skip;
		*(location + 0 + sk) = RGB.B;
		*(location + 1 + sk) = RGB.G;
		*(location + 2 + sk) = RGB.R;
	}
}

void DrawRectangle(
	struct fbjar jar,
	point top, point bot,
	color RGB
) {
	assert(CheckPIJ(jar, top));
	assert(CheckPIJ(jar, bot));

	DrawPartLine  (jar, top.y, top.x, bot.x, RGB);
	DrawPartLine  (jar, bot.y, top.x, bot.x, RGB);
	DrawPartCollum(jar, top.x, top.y, bot.y, RGB);
	DrawPartCollum(jar, bot.x, top.y, bot.y, RGB);
}

void FillRectangle (
	struct fbjar jar,
	point top, point bot,
	color RGB
) {
	uint8* location = GetFbPos(jar, top.y, top.x);
	int limy = bot.y*jar.skip;
	int limx = bot.x*jar.bpp;

	for (int i = top.y; i < limy ; i+=jar.skip ) {
		for (int j = top.x; j < limx ; j+=jar.bpp ) {
			*(location + 0 + j + i) = RGB.B;
			*(location + 1 + j + i) = RGB.G;
			*(location + 2 + j + i) = RGB.R;
		}
	}
}

//unsafe
void DrawPolarLine(
	struct fbjar jar,
	point top, polar bot,
	color RGB
) {
	polar now;
	now.a = bot.a;
	for ( now.r = 0; now.r<bot.r; now.r++) {
		point dif = SPolarToCoord(now);
		uint8* location = GetFbPos(jar, dif.y+top.y, dif.x+top.x);
		*(location+0) = RGB.B;
		*(location+1) = RGB.G;
		*(location+2) = RGB.R;
	}
}

void DrawCircle (
	struct fbjar jar,
	point top, int r,
	color RGB
) {
	point bot = MakePoint(top.y+r+r, top.x+r+r);
	assert(CheckPIJ(jar, bot));
	assert(CheckPIJ(jar, top));

	uint8* location = GetFbPos(jar, top.y, top.x);
	int sr = -(r*r);

	for (int y = -r ; y<bot.y ; y++) {
		for (int x = -r ; x<bot.x ; x++) {
			if (
					isquare(x)+isquare(y)+sr>=-r &&
					isquare(x)+isquare(y)+sr<=r
				) {
				*(location + 0 + (x+r)*jar.bpp + (y+r)*jar.skip) = RGB.G;
				*(location + 1 + (x+r)*jar.bpp + (y+r)*jar.skip) = RGB.B;
				*(location + 2 + (x+r)*jar.bpp + (y+r)*jar.skip) = RGB.R;
			}
		}
	}
}

void FillCircle (
	struct fbjar jar,
	point top, int r,
	color RGB
) {
	point bot = MakePoint(top.y+r+r, top.x+r+r);
	assert(CheckPIJ(jar, bot));
	assert(CheckPIJ(jar, top));

	uint8* location = GetFbPos(jar, top.y, top.x);
	int sr = -(r*r);

	for (int y = -r ; y<bot.y ; y++) {
		for (int x = -r ; x<bot.x ; x++) {
			if ( isquare(x)+isquare(y)+sr<=0) {
				*(location + 0 + (x+r)*jar.bpp + (y+r)*jar.skip) = RGB.G;
				*(location + 1 + (x+r)*jar.bpp + (y+r)*jar.skip) = RGB.B;
				*(location + 2 + (x+r)*jar.bpp + (y+r)*jar.skip) = RGB.R;
			}
		}
	}
}
