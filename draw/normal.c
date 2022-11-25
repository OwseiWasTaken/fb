void TDrawPartLine (
	struct fbjar jar,
	int y, int StartX, int EndX,
	color RGB,
	int thick
) {
	int skip = jar.skip;
	if (thick < 0) {
		thick = -thick;
		skip = -skip;
	}

	assert(CheckInjar(jar, y, StartX));
	assert(CheckInjar(jar, y+thick, EndX));
	assert(thick!=0);

	uint8* location = GetFbPos(jar, y, StartX);
	int Xlen = (EndX - StartX)*jar.bpp;
	int x, i;

	for (i = 0 ; i<thick ; i++) {
		for (x = 0 ; x<Xlen ; x+=jar.bpp) {
			*(location + 0 + x) = RGB.B;
			*(location + 1 + x) = RGB.G;
			*(location + 2 + x) = RGB.R;
		}
		location += skip;
	}
}

void TDrawPartCollum (
	struct fbjar jar,
	int x, int StartY, int EndY,
	color RGB,
	int thick
) {
	int skip = jar.bpp;
	if (thick < 0) {
		thick = -thick;
		skip = -skip;
	}

	assert(CheckInjar(jar, StartY, x));
	assert(CheckInjar(jar, EndY, x+thick));
	assert(thick!=0);

	uint8* location = GetFbPos(jar, StartY, x);
	int Ylen = (EndY - StartY);
	int sk, y, i;

	for (i = 0 ; i<thick ; i++) {
		sk = jar.yoff*jar.skip;
		for (y = 0 ; y<Ylen ; y++) {
			sk+=jar.skip;
			*(location + 0 + sk) = RGB.B;
			*(location + 1 + sk) = RGB.G;
			*(location + 2 + sk) = RGB.R;
		}
		location += skip;
	}
}

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

void DrawSquare (
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

void FillSquare (
	struct fbjar jar,
	point top, point bot,
	color RGB
) {
	uint8* location = GetFbPos(jar, top.y, top.x);
	int limy = bot.y*jar.skip;
	int limx = bot.x*jar.bpp;

	for (int i = 0; i < limy ; i+=jar.skip ) {
		for (int j = 0; j < limx ; j+=jar.bpp ) {
			*(location + 0 + j + i) = RGB.B;
			*(location + 1 + j + i) = RGB.G;
			*(location + 2 + j + i) = RGB.R;
		}
	}
}

