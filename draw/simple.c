// func structures
// name (
//	args
// ) {
// preprocess
//
// asserts
//
// vars
//
// run
// }
// simple funcs

//TODO: DrawAllLine/Collum

void SDrawAllLine (struct fbjar jar, int y) {
	assert(CheckInjar(jar, y, 0));

	char* location = GetFbPos(jar, y, 0);
	int x;

	for (x = 0;x<jar.skip;x+=jar.bpp) {
		*(location + x) = 255;
		*(location + 1 + x) = 255;
		*(location + 2 + x) = 255;
	}
}

void SDrawPartLine (struct fbjar jar, int y, int StartX, int EndX) {
	assert(CheckInjar(jar, y, StartX));
	assert(CheckInjar(jar, y, EndX));

	char* location = GetFbPos(jar, y, StartX);
	int Xlen = (EndX - StartX)*jar.bpp;
	int x;

	for (x = 0 ; x<Xlen ; x+=jar.bpp) {
		*(location + x) = 255;
		*(location + 1 + x) = 255;
		*(location + 2 + x) = 255;
	}
}

void SDrawPartCollum (
		struct fbjar jar,
		int x, int StartY, int EndY
) {
	assert(CheckInjar(jar, StartY, x));
	assert(CheckInjar(jar, EndY, x));

	char* location = GetFbPos(jar, StartY, x);
	int Ylen = (EndY - StartY);
	int sk = jar.yoff*jar.skip;

	for (int y = 0;y<Ylen;y++) {
		sk+=jar.skip;
		*(location + 0 + sk) = 255;
		*(location + 1 + sk) = 255;
		*(location + 2 + sk) = 255;
	}
}

void SDrawSquare (
	struct fbjar jar,
	struct Point top, struct Point bot
) {
	SDrawPartLine  (jar, top.y, top.x, bot.x);
	SDrawPartLine  (jar, bot.y, top.x, bot.x);
	SDrawPartCollum(jar, top.x, top.y, bot.y);
	SDrawPartCollum(jar, bot.x, top.y, bot.y);
}

void SFillSquare (
	struct fbjar jar,
	struct Point top, struct Point bot
) {
	assert(CheckPIJ(jar, top));
	assert(CheckPIJ(jar, bot));
	char* location = GetFbPos(jar, top.y, top.x);
	int limy = bot.y*jar.skip;
	int limx = bot.x*jar.bpp;
	for (int i = 0; i < limy ; i+=jar.skip ) {
		for (int j = 0; j < limx ; j+=jar.bpp ) {
			*(location + 0 + j + i) = 255;
			*(location + 1 + j + i) = 255;
			*(location + 2 + j + i) = 255;
		}
	}
}

