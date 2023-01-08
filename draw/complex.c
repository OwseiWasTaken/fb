void TDrawPartLine (
	struct fbjar jar,
	int y, int StartX, int EndX,
	color col,
	int thick
) {
	int skip = jar.skip;
	if (thick < 0) {
		thick = -thick;
		skip = -skip;
	}

#ifndef RELEASE
	assert(CheckInJar(jar, y, StartX));
	assert(CheckInJar(jar, y+thick, EndX));
	assert(thick!=0);
#endif

	uint8* location = GetFbPos(jar, y, StartX);
	int Xlen = (EndX - StartX)*jar.bpp;
	int x, i;

	for (i = 0 ; i<thick ; i++) {
		for (x = 0 ; x<Xlen ; x+=jar.bpp) {
			*(location + 0 + x) = col.B;
			*(location + 1 + x) = col.G;
			*(location + 2 + x) = col.R;
		}
		location += skip;
	}
}

void TDrawPartCollum (
	struct fbjar jar,
	int x, int StartY, int EndY,
	color col,
	int thick
) {
	int skip = jar.bpp;
	if (thick < 0) {
		thick = -thick;
		skip = -skip;
	}

#ifndef RELEASE
	assert(CheckInJar(jar, StartY, x));
	assert(CheckInJar(jar, EndY, x+thick));
	assert(thick!=0);
#endif

	uint8* location = GetFbPos(jar, StartY, x);
	int Ylen = (EndY - StartY);
	int sk, y, i;

	for (i = 0 ; i<thick ; i++) {
		sk = jar.yoff*jar.skip;
		for (y = 0 ; y<Ylen ; y++) {
			sk+=jar.skip;
			*(location + 0 + sk) = col.B;
			*(location + 1 + sk) = col.G;
			*(location + 2 + sk) = col.R;
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
	color col, int thick
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
				*(location+0) = col.B;
				*(location+1) = col.G;
				*(location+2) = col.R;
			}

			top.y-=y;
			top.x-=x;
		}
	}
}

void TDrawPoint (
	struct fbjar jar,
	point at, color col,
	int thick
) {
	at = pSub(at, thick/2);
#ifndef RELEASE
	assert(CheckPIJ(jar, at));
#endif

	uint8* location = GetFbPos(jar, at.y, at.x);
	for (int y = 0 ; y<thick ; y++) {
		for (int x = 0 ; x<thick ; x++) {
			location[x*jar.bpp+y*jar.skip+0] = col.B;
			location[x*jar.bpp+y*jar.skip+1] = col.G;
			location[x*jar.bpp+y*jar.skip+2] = col.R;
		}
	}
}

void TDrawpPoint (
	struct fbjar jar,
	ppoint at, color col,
	int thick
) {
	TDrawPoint(jar, pMakePoint(at), col, thick);
}

void TDrawLine (
	struct fbjar jar,
	line l, lfloat p,
	color col, int thick
) {
	for (lfloat t = 0; t < 1; t+=1/p) {
		TDrawPoint(jar, pMakePoint(D1LineLerp(l, t)), col, thick);
	}
}

void TDrawTiangle (
	struct fbjar jar,
	triangle tri, lfloat p,
	color col, int thick
) {
	for (lfloat t = 0; t <= 1; t+=1/p) {
		TDrawpPoint(jar, D1PointLerp(tri.a, tri.b, t), col, thick);
		TDrawpPoint(jar, D1PointLerp(tri.b, tri.c, t), col, thick);
		TDrawpPoint(jar, D1PointLerp(tri.c, tri.a, t), col, thick);
	}
}

