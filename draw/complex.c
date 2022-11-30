

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

	assert(CheckInJar(jar, y, StartX));
	assert(CheckInJar(jar, y+thick, EndX));
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

	assert(CheckInJar(jar, StartY, x));
	assert(CheckInJar(jar, EndY, x+thick));
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

//unsafe
// could do R+/-top
// but there are safe cases this blocks
void TDrawPolarLine(
	struct fbjar jar,
	point top, polar bot,
	color RGB, int thick
) {
	int limr = bot.r;
	top.y -= thick/2;
	top.x -= thick/2;

	for (int y = 0 ; y<thick ; y++) {
		for (int x = 0 ; x<thick ; x++) {
			top.y+=y;
			top.x+=x;

			for ( bot.r = 0; bot.r<limr; bot.r++) {
				point dif = SPolarToCoord(bot);
				uint8* location = GetFbPos(jar, dif.y+top.y, dif.x+top.x);
				*(location+0) = RGB.B;
				*(location+1) = RGB.G;
				*(location+2) = RGB.R;
			}

			top.y-=y;
			top.x-=x;
		}
	}
}
