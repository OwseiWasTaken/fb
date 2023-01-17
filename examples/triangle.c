#include "buffy.h"

int main(int argc, char *argv[]) {
	argv[0][0] = (char)argc;

	struct fbjar jar = InitBuffy();

	triangle tri = MakeSTriangle(
		MakePoint(100+200, 300+0),
		MakePoint(100+350, 300-50),
		MakePoint(100+400, 300+200)
	);

	for (lfloat p = 0; p<1001; p++) {
		printf("\x1b[2K\b\b\b%3.0Lf", p);
		fflush(stdout);
		TDrawTiangle(jar, tri, p, RGB(0,0,0), 2);
		SDrawTiangle(jar, tri, p);
  
		TDrawPoint(jar, tri.a, RGB(255,0,0), 3);
		TDrawPoint(jar, tri.b, RGB(0,255,0), 3);
		TDrawPoint(jar, tri.c, RGB(0,0,255), 3);
  
		fsleep(0.014);
	}

	StopBuffy(jar);
	return 0;
}
