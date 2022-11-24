// advanced version of normal funcs

void ADrawPartLine (
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

	char* location = GetFbPos(jar, y, StartX);
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

void ADrawPartCollum (
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

	char* location = GetFbPos(jar, StartY, x);
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
