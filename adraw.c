// advanced version of normal funcs

// storing random shit here

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

	char* location = jar.fbmem+GetPixelPos(jar, y, StartX);
	int Xlen = (EndX - StartX)*jar.bpp;
	int x, i;

	for (i = 0 ; i<thick ; i++) {
		location += jar.skip;
		for (x = 0 ; x<Xlen ; x+=jar.bpp) {
			*(location + 0 + x) = RGB.B;
			*(location + 1 + x) = RGB.G;
			*(location + 2 + x) = RGB.R;
		}
	}
}


