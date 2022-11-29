#include "buffy.h"

int main (void) {
	struct fbjar jar = InitBuffy();
	bitmap b;
	b.heigth = 8;
	b.width = 1;
	uint8 cont[] = {
		254,
		192,
		160,
		144,
		136,
		132,
		2,
		1
	};
	b.cont = cont;
	point at = MakePoint(100,100);
	DrawBitmap(jar, b, at);
	sleep(3);
}
