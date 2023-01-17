void DrawAllLine (
	struct fbjar jar, uint y, color RGB
) {
#ifndef RELEASE
	assert(CheckInJar(jar, y, 0));
#endif

	uint8* location = GetFbPos(jar, y, 0);
	uint x;

	for (x = 0;x<jar.cols;x++) {
		location[0 + x*jar.bpp] = RGB.B;
		location[1 + x*jar.bpp] = RGB.G;
		location[2 + x*jar.bpp] = RGB.R;
	}
}

void DrawAllCollum (
	struct fbjar jar, uint x, color RGB
) {
#ifndef RELEASE
	assert(CheckInJar(jar, 0, x));
#endif

	uint8* location = GetFbPos(jar, 0, x);
	uint y;

	for (y = 0;y<jar.rows;x++) {
		location[0 + x*jar.skip] = RGB.B;
		location[1 + x*jar.skip] = RGB.G;
		location[2 + x*jar.skip] = RGB.R;
	}
}

void DrawPartLine (
	struct fbjar jar,
	uint y, uint StartX, uint EndX,
	color RGB
) {
#ifndef RELEASE
	assert(CheckInJar(jar, y, StartX));
	assert(CheckInJar(jar, y, EndX));
#endif

	uint8* location = GetFbPos(jar, y, StartX);
	uint Xlen = (EndX - StartX)*jar.bpp;
	uint x;

	for (x = 0 ; x<Xlen ; x+=jar.bpp) {
		*(location + 0 + x) = RGB.B;
		*(location + 1 + x) = RGB.G;
		*(location + 2 + x) = RGB.R;
	}
}

void DrawPartCollum (
	struct fbjar jar,
	uint x, uint StartY, uint EndY,
	color RGB
) {
#ifndef RELEASE
	assert(CheckInJar(jar, StartY, x));
	assert(CheckInJar(jar, EndY, x));
#endif

	uint8* location = GetFbPos(jar, StartY, x);
	uint Ylen = (EndY - StartY);
	uint sk = jar.yoff*jar.skip;

	for (uint y = 0;y<Ylen;y++) {
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
#ifndef RELEASE
	assert(CheckPIJ(jar, top));
	assert(CheckPIJ(jar, bot));
#endif

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
#ifndef RELEASE
	assert(CheckInJar(jar, bot.y+top.y, bot.x+top.x));
	assert(CheckPIJ(jar, bot));
#endif
	uint8* location = GetFbPos(jar, top.y, top.x);
	uint limy = bot.y*jar.skip;
	uint limx = bot.x*jar.bpp;

	for (uint i = 0; i < limy ; i+=jar.skip ) {
		for (uint j = 0; j < limx ; j+=jar.bpp ) {
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
#ifndef RELEASE
	assert(CheckPIJ(jar, top));
	assert(CheckPIJ(jar, bot));
#endif
	uint8* location = GetFbPos(jar, top.y, top.x);
	uint limy = bot.y-top.y;
	uint limx = bot.x-top.x;

	for (uint i = 0; i < limy ; i++ ) {
		for (uint j = 0; j < limx ; j++ ) {
			location[0 + j*jar.bpp + i*jar.skip] = RGB.B;
			location[1 + j*jar.bpp + i*jar.skip] = RGB.G;
			location[2 + j*jar.bpp + i*jar.skip] = RGB.R;
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
#ifndef RELEASE
	assert(CheckPIJ(jar, bot));
	assert(CheckPIJ(jar, top));
#endif

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
#ifndef RELEASE
	assert(CheckPIJ(jar, bot));
	assert(CheckPIJ(jar, top));
#endif

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

void DrawBitmap (
	struct fbjar jar, bitmap bmap, point top, color RGB
) {
#ifndef RELEASE
	assert(CheckPIJ(jar, top));
	assert(CheckInJar(jar, top.y+bmap.heigth, top.x+bmap.width));
	assert(bmap.cont!=NULL);
#endif
	uint8* location = GetFbPos(jar, top.y, top.x);
	uint rollback = jar.bpp*(bmap.width);
	for (uint i = 0 ; i<(bmap.heigth); i++) {
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

void ApplyBitmap (
	struct fbjar jar, bitmap bmap, point top, color RGB
) {
#ifndef RELEASE
	assert(CheckPIJ(jar, top));
	assert(CheckInJar(jar, top.y+bmap.heigth, top.x+bmap.width));
	assert(bmap.cont);
#endif
	uint8* location = GetFbPos(jar, top.y, top.x);
	for (uint i = 0 ; i<(bmap.heigth); i++) {
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

//TODO: vars.c
bytesmap SaveBytes (
	struct fbjar jar, point top, point bot
) {
#ifndef RELEASE
	assert(CheckPIJ(jar, top));
	assert(CheckPIJ(jar, bot));
#endif

	uint limy = bot.y-top.y;
	uint limx = bot.x-top.x;
	uint size = limy*limx+1;
	uint8* location = GetFbPnt(jar, top);

	bytesmap map;
	color* cols = malloc(size * sizeof(color));
	for (uint i = 0; i < limy; i++ ) {
		for (uint j = 0; j < limx; j++ ) {
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

bytesmap RSaveBytes ( struct fbjar jar, point top, upoint len) {
#ifndef RELEASE
	assert(CheckPIJ(jar, top));
	assert(CheckuPIJ(jar, len));
#endif

	uint size = len.y*len.x;
	uint8* location = GetFbPnt(jar, top);

	bytesmap map;
	color* cols = malloc(size * sizeof(color));
	for (uint i = 0; i < len.y; i++ ) {
		for (uint j = 0; j < len.x; j++ ) {
			cols[0+i*len.x+j].B = location[0 + jar.skip*i + jar.bpp*j];
			cols[1+i*len.x+j].G = location[1 + jar.skip*i + jar.bpp*j];
			cols[2+i*len.x+j].R = location[2 + jar.skip*i + jar.bpp*j];
		}
	}
	map.cont = cols;
	map.heigth = (uint)len.y;
	map.width = (uint)len.x;
	return map;
}

void DrawBytesmap (
	struct fbjar jar, bytesmap map, point top
) {
	point len = MakePoint(map.heigth, map.width);
#ifndef RELEASE
	assert(CheckPIJ(jar, top));
	assert(CheckInJar(jar, map.heigth+top.y, map.width+top.x));
#endif

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

void ApplyBytesmap (
	struct fbjar jar, bytesmap map, point top
) {
	point len = MakePoint((int)map.heigth, (int)map.width);
#ifndef RELEASE
	assert(CheckPIJ(jar, top));
	assert(CheckInJar(jar, map.heigth+top.y, map.width+top.x));
#endif

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

// RGB
void DrawBytemap (
	struct fbjar jar, bytemap bmap, point top, uint chan
) {
	uint8* location = GetFbPos(jar, top.y, top.x);
	uint chanb, chang, chanr;

	// direct info into correct chan or alpha chan
	chanr = (chan&CHAN_R)>>2?2:3;
	chang = (chan&CHAN_G)>>1?1:3;
	chanb = (chan&CHAN_B)>>0?0:3;

	for (uint i = 0 ; i<(bmap.heigth); i++) {
		for (uint j = 0 ; j<(bmap.width) ; j++) {
			location[chanb] = bmap.cont[i*bmap.width+j];
			location[chang] = bmap.cont[i*bmap.width+j];
			location[chanr] = bmap.cont[i*bmap.width+j];
			location += jar.bpp;
		}
		location -= jar.bpp*(bmap.width);
		location += jar.skip;
	}
}

void ApplyBytemap (
	struct fbjar jar, bytemap bmap, point top, uint chan
) {
	uint8* location = GetFbPos(jar, top.y, top.x);
	uint chanb, chang, chanr;
	chanr = (chan&CHAN_R)>>2?2:3;
	chang = (chan&CHAN_G)>>1?1:3;
	chanb = (chan&CHAN_B)>>0?0:3;
	for (uint i = 0 ; i<(bmap.heigth); i++) {
		for (uint j = 0 ; j<(bmap.width) ; j++) {
			if (bmap.cont[i*bmap.width+j]) {
				location[chanb] = bmap.cont[i*bmap.width+j];
				location[chang] = bmap.cont[i*bmap.width+j];
				location[chanr] = bmap.cont[i*bmap.width+j];
			}
			location += jar.bpp;
		}
		location -= jar.bpp*(bmap.width);
		location += jar.skip;
	}
}

void DrawPoint (
	struct fbjar jar, point at, color col
) {
#ifndef RELEASE
	assert(CheckPIJ(jar, at));
#endif

	uint8* location = GetFbPos(jar, at.y, at.x);
	location[0] = col.B;
	location[1] = col.G;
	location[2] = col.R;
}

void DrawpPoint (
	struct fbjar jar, ppoint at, color col
) {
	DrawPoint(jar, pMakePoint(at), col);
}

// precision = how many pixels will be drawn (p+1)
void DrawLine (
	struct fbjar jar, line l, lfloat p, color col
) {
	for (lfloat t = 0; t < 1; t+=1/p) {
		DrawPoint(jar, pMakePoint(D1LineLerp(l, t)), col);
	}
}

void pDrawLine (
	struct fbjar jar, point a, point b, lfloat p, color col
) {
	for (lfloat t = 0; t < 1; t+=1/p) {
		DrawPoint(jar, pMakePoint(D1PointLerp(a, b, t)), col);
	}
}

void DrawTiangle (
	struct fbjar jar, triangle tri, lfloat p, color col
) {
	for (lfloat t = 0; t <= 1; t+=1/p) {
		DrawpPoint(jar, D1PointLerp(tri.a, tri.b, t), col);
		DrawpPoint(jar, D1PointLerp(tri.b, tri.c, t), col);
		DrawpPoint(jar, D1PointLerp(tri.c, tri.a, t), col);
	}
}

