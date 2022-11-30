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
	point at = MakePoint(100,1000);
	point end = MakePoint(at.y+b.heigth, at.x+8*b.width);
	SDrawBitmap(jar, b, at);
	back = KeepRectangle(jar, at, end);
}
