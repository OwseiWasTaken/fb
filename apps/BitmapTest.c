#include "buffy.h"

void ShowByteMap(bytesmap m);

int main (int argc, char** argv) {
	struct fbjar jar = InitBuffy();
	bytesmap b;
	b.heigth = 8;
	b.width = 1;
	color cont[] = {
		RGB(255,0,127),
		RGB(255,0,127),
		RGB(255,0,127),
		RGB(255,0,127),
		RGB(255,0,127),
		RGB(255,0,127),
		RGB(255,0,127),
		RGB(255,0,127)
	};
	b.cont = cont;
	point at = MakePoint(100,1000);
	point end = MakePoint(at.y+b.heigth, at.x+b.width);
	if (argc-1) {
		DrawBytesmap(jar, b, at);
		//SDrawBitmap(jar, b, at);
	}
	//printf("%ld\n", sizeof(color));
	bytesmap back = SaveBytes(jar, at, end);
	printf("ByteMap size: %dX%d\n", back.heigth, back.width);
	printf("ByteMap size: %dX%d\n", b.heigth, b.width);
	ShowByteMap(b);
}

void ShowByteMap(bytesmap m) {
	for (int i = 0 ; i<m.heigth ; i++) {
		for (int j = 0 ; j<m.width ; j++) {
			printf("%.3d,%.3d,%.3d, ",
				m.cont[i*m.width+j].R,
				m.cont[i*m.width+j].G,
				m.cont[i*m.width+j].B
			);
		}
		putc('\n', stdout);
	}
}
