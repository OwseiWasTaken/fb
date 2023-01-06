#include "buffy.h"

// this is wrong
// it lerps thru every line once,
// insted of halfing the number of lines until a single point is reached
inline ppoint DNLineLerp(line lines[], int lnc /* line count */, lfloat t) {
	line rn = lines[0];
	for (int i = 1; i<lnc; i++) {
		rn = pMakeLine(
			D1LineLerp(rn, t),
			D1LineLerp(lines[i], t)
		);
	}
	return D1LineLerp(rn, t);
}

inline ppoint D2LineLerp(point spot[], lfloat t) {
	return D1pLineLerp(
	MakepLine(
		D1PointLerp(spot[0], spot[1], t),
		D1PointLerp(spot[1], spot[2], t)
	) , t);
}

inline ppoint BezierTriangleLerp(triangle tri, lfloat t) {
	return D1pLineLerp(
	MakepLine(
		D1PointLerp(tri.a, tri.b, t),
		D1PointLerp(tri.b, tri.c, t)
	) , t);
}

int main(int argc, char *argv[]) {
	struct fbjar jar = InitBuffy();

	printf("%d%s\n", argc, *argv);
	color pink = RGB(255,128,128);

	triangle tr = MakeTriangle(
		MakePoint(300+ 00,600+ 20),
		MakePoint(300+ 20,600+ 20),
		MakePoint(300+ 20,600+ 40)
	);
	point tl[] = {
		MakePoint(300+ 20,600+ 00),
		MakePoint(300+ 20,600+ 20),
		MakePoint(300+ 00,600+ 20)
	};
	point bl[] = {
		MakePoint(300+ 40,600+ 20),
		MakePoint(300+ 20,600+ 20),
		MakePoint(300+ 20,600+ 00)
	};
	point br[] = {
		MakePoint(300+ 20,600+ 40),
		MakePoint(300+ 20,600+ 20),
		MakePoint(300+ 40,600+ 20),
	};

	lfloat p = 600;

	for (lfloat t = 0.0f; t < 3; t+=1/p) {
		DrawpPoint(jar, BezierTriangleLerp(tr, t), pink);
		DrawpPoint(jar, D2LineLerp(tl, t), pink);
		DrawpPoint(jar, D2LineLerp(bl, t), pink);
		DrawpPoint(jar, D2LineLerp(br, t), pink);

		usleep(3000);
	}

	StopBuffy(jar);
	return 0;
}
