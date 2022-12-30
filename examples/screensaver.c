#include "buffy.h"

point GetSpeed() {
	return SPolarToCoord( MakePolar(rand()%10, rand()%360) );
}

int main ( int argc, char **argv ) {
	struct fbjar jar = InitBuffy();
	srand(time(NULL));
	const color erase = RGB(255,255,255);

	point speed;
	do {
		speed = GetSpeed();
	} while (!(speed.y&speed.x)); // make sure speed ain't 0
	point ball = MakePoint(jar.rows/2, jar.cols/2);

	while (1) {
		DrawPoint(jar, ball, erase);
		ball = pMovePoint(ball, speed); // to ball, apply speed
		if (!CheckPIJ(jar, ball)) {
			if (ball.x >= jar.cols || ball.x < 0) {
				speed.x = -speed.x;
			}
			if (ball.y >= jar.rows || ball.y < 0) {
				speed.y = -speed.y;
			}
			ball = pMovePoint(ball, speed); // to ball, apply speed
		}
		SDrawPoint(jar, ball);
		usleep(30000);
	}

	CloseFb(jar);
	return 0;
}
