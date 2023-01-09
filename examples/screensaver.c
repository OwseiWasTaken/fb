#include "buffy.h"

polar GetSpeed() {
	return MakePolar(rand()%10, rand()%360);
}

int main ( int argc, char **argv ) {
	struct fbjar jar = InitBuffy();
	srand(time(NULL));
	color trail = RGB(0,0,0);
	color PaintBall = RGB(255,255,255);
	int slp = 30000;

	ppoint speed;
	polar aspd;
	ppoint ball = MakepPoint((float)jar.rows/2, (float)jar.cols/2);

	do {
		aspd = GetSpeed();
	} while (!(aspd.r)); // make sure speed ain't 0

	for (int i = 1; i<argc; i++) {
		if (!strcmp(argv[i], "-s")) {
			i++;
			assert(i < argc);
			aspd.r = atoi(argv[i]);
		} else if (!strcmp(argv[i], "-t")) {
			i++;
			assert(i < argc);
			slp = atoi(argv[i]);
		} else if (!strcmp(argv[i], "--ang")) {
			i++;
			assert(i < argc);
			aspd.a = atoi(argv[i]);
		} else if (!strcmp(argv[i], "--tr")) {
			i++;
			assert(i < argc);
			trail.R = atoi(argv[i]);
		} else if (!strcmp(argv[i], "--tg")) {
			i++;
			assert(i < argc);
			trail.G = atoi(argv[i]);
		} else if (!strcmp(argv[i], "--tb")) {
			i++;
			assert(i < argc);
			trail.B = atoi(argv[i]);
		} else if (!strcmp(argv[i], "--br")) {
			i++;
			assert(i < argc);
			PaintBall.R = atoi(argv[i]);
		} else if (!strcmp(argv[i], "--bg")) {
			i++;
			assert(i < argc);
			PaintBall.G = atoi(argv[i]);
		} else if (!strcmp(argv[i], "--bb")) {
			i++;
			assert(i < argc);
			PaintBall.B = atoi(argv[i]);
		}
	}
	speed = PolarTopCoord(aspd, __BUFFY_H_POINT_ZZ);
#ifndef RELEASE
	printf("r:%Lf, %Lf°\n", aspd.r, aspd.a);
#endif
	HideCursor();
	// 3°30
	// 32°5

	while (1) {
		ball = ppMovepPoint(ball, speed); // to ball, apply speed
		if (!CheckpPIJ(jar, ball)) {
			if (ball.x >= jar.cols || ball.x < 0) {
				speed.x = -speed.x;
			}
			if (ball.y >= jar.rows || ball.y < 0) {
				speed.y = -speed.y;
			}
			ball = ppMovepPoint(ball, speed); // to ball, apply speed
		}
		if (CheckpPIJ(jar, ball)) {
			DrawpPoint(jar, ball, PaintBall);
			if (slp) usleep(slp);
			DrawpPoint(jar, ball, trail);
		}
	}

	CloseFb(jar);
	return 0;
}
