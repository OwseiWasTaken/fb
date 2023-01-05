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

int main(int argc, char *argv[]) {
	struct fbjar jar = InitBuffy();

	printf("%d%s\n", argc, *argv);
	color pink = RGB(255,128,128);

	point tr[] = {
		MakePoint(100+ 000,100+ 200),
		MakePoint(100+ 200,100+ 200),
		MakePoint(100+ 200,100+ 400)
	};
	point tl[] = {
		MakePoint(100+ 200,100+ 000),
		MakePoint(100+ 200,100+ 200),
		MakePoint(100+ 000,100+ 200)
	};
	point bl[] = {
		MakePoint(100+ 400,100+ 200),
		MakePoint(100+ 200,100+ 200),
		MakePoint(100+ 200,100+ 000)
	};
	point br[] = {
		MakePoint(100+ 200,100+ 400),
		MakePoint(100+ 200,100+ 200),
		MakePoint(100+ 400,100+ 200),
	};

	lfloat p = 600;

	for (lfloat t = 0.0f; t < 1.01; t+=1/p) {
		DrawpPoint(jar, D2LineLerp(tr, t), pink);
		DrawpPoint(jar, D2LineLerp(tl, t), pink);
		DrawpPoint(jar, D2LineLerp(bl, t), pink);
		DrawpPoint(jar, D2LineLerp(br, t), pink);

		usleep(8000);
	}

	StopBuffy(jar);
	return 0;
}
