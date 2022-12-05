void DrawAllLine (
	struct fbjar jar, int y, color RGB
) {
	assert(CheckInJar(jar, y, 0));

	uint8* location = GetFbPos(jar, y, 0);
	int x;

	for (x = 0;x<jar.rows;x++) {
		location[0 + x*jar.bpp] = RGB.B;
		location[1 + x*jar.bpp] = RGB.G;
		location[2 + x*jar.bpp] = RGB.R;
	}
}

void DrawAllCollum (
	struct fbjar jar, int x, color RGB
) {
	assert(CheckInJar(jar, 0, x));

	uint8* location = GetFbPos(jar, 0, x);
	int y;

	for (y = 0;y<jar.cols;x++) {
		location[0 + x*jar.skip] = RGB.B;
		location[1 + x*jar.skip] = RGB.G;
		location[2 + x*jar.skip] = RGB.R;
	}
}

void DrawPartLine (
	struct fbjar jar,
	int y, int StartX, int EndX,
	color RGB
) {
	assert(CheckInJar(jar, y, StartX));
	assert(CheckInJar(jar, y, EndX));

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
	assert(CheckInJar(jar, StartY, x));
	assert(CheckInJar(jar, EndY, x));

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

// R for relative
void RFillRectangle (
	struct fbjar jar,
	point top, point bot,
	color RGB
) {
	assert(CheckInJar(jar, bot.y+top.y, bot.x+top.x));
	assert(CheckPIJ(jar, bot));
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

void FillRectangle (
	struct fbjar jar,
	point top, point bot,
	color RGB
) {
	uint8* location = GetFbPos(jar, top.y, top.x);
	int limy = (top.y-bot.y)*jar.skip;
	int limx = (top.x-bot.x)*jar.bpp;

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
				location[0+(x+r)*jar.bpp + (y+r)*jar.skip] = RGB.B;
				location[1+(x+r)*jar.bpp + (y+r)*jar.skip] = RGB.G;
				location[2+(x+r)*jar.bpp + (y+r)*jar.skip] = RGB.R;
			}
		}
	}
}

void DrawBitmap (struct fbjar jar, bitmap bmap, point top, color RGB) {
	assert(CheckPIJ(jar, top));
	assert(CheckInJar(jar, top.y+bmap.heigth, top.x+bmap.width));
	assert(bmap.cont!=NULL);
	uint8* location = GetFbPos(jar, top.y, top.x);
	int rollback = jar.bpp*(bmap.width);
	for (int i = 0 ; i<(bmap.heigth); i++) {
		for (uint j = 0 ; j<(bmap.width) ; j++) {
			location[0] = bmap.cont[i*bmap.width+j]*RGB.B;
			location[1] = bmap.cont[i*bmap.width+j]*RGB.G;
			location[2] = bmap.cont[i*bmap.width+j]*RGB.R;
			location += jar.bpp;
		}
		location -= rollback;
		location += jar.skip;
	}
}

void ApplyBitmap (struct fbjar jar, bitmap bmap, point top, color RGB) {
	assert(CheckPIJ(jar, top));
	assert(CheckInJar(jar, top.y+bmap.heigth, top.x+bmap.width));
	assert(bmap.cont);
	uint8* location = GetFbPos(jar, top.y, top.x);
	for (int i = 0 ; i<(bmap.heigth); i++) {
		for (uint j = 0 ; j<(bmap.width) ; j++) {
			if (bmap.cont[i*bmap.width+j]) {
				location[0] = RGB.B;
				location[1] = RGB.G;
				location[2] = RGB.R;
			}
			location += jar.bpp;
		}
		location -= jar.bpp*(bmap.width);
		location += jar.skip;
	}
}

bytesmap SaveBytes ( struct fbjar jar, point top, point bot) {
	assert(CheckPIJ(jar, top));
	assert(CheckPIJ(jar, bot));

	int limy = bot.y-top.y;
	int limx = bot.x-top.x;
	int size = limy*limx+1;
	uint8* location = GetFbPnt(jar, top);

	bytesmap map;
	color* cols = malloc(size * sizeof(color));
	for (int i = 0; i < limy; i++ ) {
		for (int j = 0; j < limx; j++ ) {
			cols[0+i*limx+j].B = location[0 + jar.skip*i + jar.bpp*j];
			cols[0+i*limx+j].G = location[1 + jar.skip*i + jar.bpp*j];
			cols[0+i*limx+j].R = location[2 + jar.skip*i + jar.bpp*j];
		}
	}
	map.cont = cols;
	map.heigth = limy;
	map.width = limx;
	return map;
}

bytesmap RSaveBytes ( struct fbjar jar, point top, point len) {
	assert(CheckPIJ(jar, top));
	assert(CheckPIJ(jar, len));

	int size = len.y*len.x;
	uint8* location = GetFbPnt(jar, top);

	bytesmap map;
	color* cols = malloc(size * sizeof(color));
	for (int i = 0; i < len.y; i++ ) {
		for (int j = 0; j < len.x; j++ ) {
			cols[0+i*len.x+j].B = location[0 + jar.skip*i + jar.bpp*j];
			cols[1+i*len.x+j].G = location[1 + jar.skip*i + jar.bpp*j];
			cols[2+i*len.x+j].R = location[2 + jar.skip*i + jar.bpp*j];
		}
	}
	map.cont = cols;
	map.heigth = len.y;
	map.width = len.x;
	return map;
}

void DrawBytesmap ( struct fbjar jar, bytesmap map, point top) {
	point len = MakePoint(map.heigth, map.width);
	assert(CheckPIJ(jar, top));
	assert(CheckInJar(jar, map.heigth+top.y, map.width+top.x));

	uint8* location = GetFbPnt(jar, top);
	color* cols = map.cont;

	for (int i = 0; i < len.y; i++ ) {
		for (int j = 0; j < len.x; j++ ) {
			location[0 + jar.skip*i + jar.bpp*j] = cols[0+i*len.x+j].B;
			location[1 + jar.skip*i + jar.bpp*j] = cols[0+i*len.x+j].G;
			location[2 + jar.skip*i + jar.bpp*j] = cols[0+i*len.x+j].R;
		}
	}
}

void ApplyBytesmap ( struct fbjar jar, bytesmap map, point top) {
	point len = MakePoint(map.heigth, map.width);
	assert(CheckPIJ(jar, top));
	assert(CheckInJar(jar, map.heigth+top.y, map.width+top.x));

	uint8* location = GetFbPnt(jar, top);
	color* cols = map.cont;
	color RGB;

	for (int i = 0; i < len.y; i++ ) {
		for (int j = 0; j < len.x; j++ ) {
			RGB = cols[0+i*len.x+j];
			if (RGB.R|RGB.G|RGB.B) {
				location[0 + jar.skip*i + jar.bpp*j] = RGB.B;
				location[1 + jar.skip*i + jar.bpp*j] = RGB.G;
				location[2 + jar.skip*i + jar.bpp*j] = RGB.R;
			}
		}
	}
}
