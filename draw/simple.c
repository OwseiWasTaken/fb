void SDrawAllLine (
	struct fbjar jar, uint y
) {
	assert(CheckInJar(jar, y, 0));

	uint8* location = GetFbPos(jar, y, 0);

	memset(location, 255, jar.cols*jar.bpp);
}

void SDrawAllCollum (
	struct fbjar jar, uint x
) {
	assert(CheckInJar(jar, 0, x));

	uint8* location = GetFbPos(jar, 0, x);
	uint y;

	for (y = 0;y<jar.rows;y++) {
		location[0 + y*jar.skip] = 255;
		location[1 + y*jar.skip] = 255;
		location[2 + y*jar.skip] = 255;
	}
}

void SDrawPartLine (struct fbjar jar, uint y, uint StartX, uint EndX) {
	assert(CheckInJar(jar, y, StartX));
	assert(CheckInJar(jar, y, EndX));

	uint8* location = GetFbPos(jar, y, StartX);
	uint Xlen = (EndX - StartX)*jar.bpp;
	uint x;

	for (x = 0 ; x<Xlen ; x+=jar.bpp) {
		*(location + x) = 255;
		*(location + 1 + x) = 255;
		*(location + 2 + x) = 255;
	}
}

void SDrawPartCollum (
		struct fbjar jar,
		uint x, uint StartY, uint EndY
) {
	assert(CheckInJar(jar, StartY, x));
	assert(CheckInJar(jar, EndY, x));

	uint8* location = GetFbPos(jar, StartY, x);
	uint Ylen = (EndY - StartY);
	uint sk = jar.yoff*jar.skip;

	for (uint y = 0;y<Ylen;y++) {
		sk+=jar.skip;
		*(location + 0 + sk) = 255;
		*(location + 1 + sk) = 255;
		*(location + 2 + sk) = 255;
	}
}

void SDrawSquare (
	struct fbjar jar,
	point top, point bot
) {
	SDrawPartLine  (jar, top.y, top.x, bot.x);
	SDrawPartLine  (jar, bot.y, top.x, bot.x);
	SDrawPartCollum(jar, top.x, top.y, bot.y);
	SDrawPartCollum(jar, bot.x, top.y, bot.y);
}

void SRFillRectangle (
	struct fbjar jar,
	point top, point bot
) {
	assert(CheckInJar(jar, bot.y+top.y, bot.x+top.x));
	assert(CheckPIJ(jar, top));
	uint8* location = GetFbPos(jar, top.y, top.x);
	uint limy = (uint)bot.y*jar.skip;
	uint limx = (uint)bot.x*jar.bpp;

	for (uint i = 0; i < limy ; i+=jar.skip ) {
		memset(location, 255, jar.bpp*limx);
	}
}

void SFillRectangle (
	struct fbjar jar,
	point top, point bot
) {
	assert(CheckPIJ(jar, top));
	assert(CheckPIJ(jar, bot));
	uint8* location = GetFbPos(jar, top.y, top.x);
	uint limy = (uint)(top.y-bot.y)*jar.skip;
	uint limx = (uint)(top.x-bot.x)*jar.bpp;
	for (uint i = 0; i < limy ; i+=jar.skip ) {
		memset(location, 255, jar.bpp*limx);
	}
}

void SDrawPolarLine(
	struct fbjar jar,
	point top, polar bot
) {
	polar now;
	now.a = bot.a;
	for ( now.r = 0; now.r<bot.r; now.r++) {
		point dif = SPolarToCoord(now);
		uint8* location = GetFbPos(jar, dif.y+top.y, dif.x+top.x);
		*(location+0) = 255;
		*(location+1) = 255;
		*(location+2) = 255;
	}
}

void SDrawCircle (
	struct fbjar jar,
	point top, int r
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
				location[0 + (x+r)*jar.bpp + (y+r)*jar.skip] = 255;
				location[1 + (x+r)*jar.bpp + (y+r)*jar.skip] = 255;
				location[2 + (x+r)*jar.bpp + (y+r)*jar.skip] = 255;
			}
		}
	}
}

void SFillCircle (
	struct fbjar jar,
	point top, int r
) {
	point bot = MakePoint(top.y+r+r, top.x+r+r);
	assert(CheckPIJ(jar, bot));
	assert(CheckPIJ(jar, top));

	uint8* location = GetFbPos(jar, top.y, top.x);
	int sr = -(r*r);

	for (int y = -r ; y<bot.y ; y++) {
		for (int x = -r ; x<bot.x ; x++) {
			if ( isquare(x)+isquare(y)+sr<=0) {
				location[0+(x+r)*jar.bpp + (y+r)*jar.skip] = 255;
				location[1+(x+r)*jar.bpp + (y+r)*jar.skip] = 255;
				location[2+(x+r)*jar.bpp + (y+r)*jar.skip] = 255;
			}
		}
	}
}

//unsanfe
void SDrawBitmap (struct fbjar jar, bitmap bmap, point top) {
	assert(CheckPIJ(jar, top));
	//TODO make map funcs safe
	//assert(CheckPIJ(jar, bot));

	uint8* location = GetFbPos(jar, top.y, top.x);
	for (uint i = 0 ; i<(bmap.heigth); i++) {
		for (uint j = 0 ; j<(bmap.width) ; j++) {
			location[0] = bmap.cont[i*bmap.width+j]*255;
			location[1] = bmap.cont[i*bmap.width+j]*255;
			location[2] = bmap.cont[i*bmap.width+j]*255;
			location += jar.bpp;
		}
		location -= jar.bpp*(bmap.width);
		location += jar.skip;
	}
}

//unsanfe
void SApplyBitmap (struct fbjar jar, bitmap bmap, point top) {
	uint8* location = GetFbPos(jar, top.y, top.x);
	for (uint i = 0 ; i<(bmap.heigth); i++) {
		for (uint j = 0 ; j<(bmap.width) ; j++) {
			if (bmap.cont[i*bmap.width+j]) {
				location[0] = 255;
				location[1] = 255;
				location[2] = 255;
			}
			location += jar.bpp;
		}
		location -= jar.bpp*(bmap.width);
		location += jar.skip;
	}
}

//unsanfe
void SDrawBytemap (struct fbjar jar, bytemap bmap, point top) {
	uint8* location = GetFbPos(jar, top.y, top.x);
	for (uint i = 0 ; i<(bmap.heigth); i++) {
		for (uint j = 0 ; j<(bmap.width) ; j++) {
			location[0] = bmap.cont[0];
			location[1] = bmap.cont[0];
			location[2] = bmap.cont[0];
			location += jar.bpp;
			bmap.cont++;
		}
		location -= jar.bpp*(bmap.width);
		location += jar.skip;
	}
}

//unsanfe
void SApplyBytemap (struct fbjar jar, bytemap bmap, point top) {
	uint8* location = GetFbPos(jar, top.y, top.x);
	for (uint i = 0 ; i<(bmap.heigth); i++) {
		for (uint j = 0 ; j<(bmap.width) ; j++) {
			if (bmap.cont[i*bmap.width+j]) {
				location[0] = bmap.cont[i*bmap.width+j];
				location[1] = bmap.cont[i*bmap.width+j];
				location[2] = bmap.cont[i*bmap.width+j];
			}
			location += jar.bpp;
		}
		location -= jar.bpp*(bmap.width);
		location += jar.skip;
	}
}

void SDrawPoint (struct fbjar jar, point at) {
	assert(CheckPIJ(jar, at));

	uint8* location = GetFbPos(jar, at.y, at.x);
	location[0] = 255;
	location[1] = 255;
	location[2] = 255;
}
