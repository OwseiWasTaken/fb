#include "buffy.h"

int main(int argc, char *argv[]) {
	argv[0][0] = (char)argc;

	struct fbjar jar = InitBuffy();

	triangle tri = MakeSTriangle(
		MakePoint(700, 100+0),
		MakePoint(100+300, 100+0),
		MakePoint(100+400, 100+600)
	);

	printf("%f,", GetDistance(tri.a, tri.b));
	printf("%f\n", GetADistance(tri.a, tri.b));

	printf("%f,", GetDistance(tri.b, tri.c));
	printf("%f\n", GetADistance(tri.b, tri.c));

	printf("%f,", GetDistance(tri.c, tri.a));
	printf("%f\n", GetADistance(tri.c, tri.a));
	printf("%f\n",
		(GetDistance(tri.a, tri.b)+
		GetDistance(tri.b, tri.c)+
		GetDistance(tri.c, tri.a))/3
	);

	for (lfloat p = 0; p<1001; p++) {
		printf("\x1b[2K\b\b\b%3.0Lf", p);
		fflush(stdout);
		TDrawTiangle(jar, tri, p, RGB(0,0,0), 2);
		SDrawTiangle(jar, tri, p);
		fsleep(0.014);
	}

	point hip = pMakePoint(D1PointLerp(tri.a, tri.c, 0.5));
	ppoint mid = D1PointLerp(hip, tri.b, 0.5);
	SDrawpPoint(jar, mid);

	StopBuffy(jar);
	return 0;
}
