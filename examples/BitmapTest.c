#include "buffy.h"

void ShowByteMap(bytesmap m);

int main (int argc, char** argv) {
	struct fbjar jar = InitBuffy();

	//bitmap b;
	//b.heigth = 2;
	//b.width = 8;
	//bool cont[] = {
	//	1,0,1,0,1,0,1,0,
	//	0,1,0,1,0,1,0,1
	//};
	//b.cont = cont;
	//SDrawBitmap(jar, b, MakePoint(300,300));
	bytemap b;
	b.heigth = 8;
	b.width = 3;
	byte cont[] = {
		255,0,127,
		255,0,127,
		255,0,127,
		255,0,127,
		255,0,127,
		255,0,127,
		255,0,127,
		255,0,127
	};
	b.cont = cont;
	point at = MakePoint(100,1000);
	if (argc-1) {
		DrawBytemap(jar, b, at);
	}
}
