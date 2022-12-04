#include "buffy.h"

// 8 = released
// 8+2 =
// 8+4 = middle

int main(int argc, char* argv[]){
	struct fbjar jar = InitBuffy();
	int mice = open("/dev/input/mice", O_RDONLY);
	assert(mice != -1);
	printf("reading mouse from fd%d\n", mice);

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
	color green = RGB(0,255,0);
	point loc = MakePoint(100, 100);

	uint8 buff[3] = {0};
	int8 ry, rx;
	bool left, right, middle;
	int bytes;

	while (1) {
		bytes = read(mice, buff, 3);
		if (bytes) {
			buff[0]-=8;
			left   = (buff[0]&1)==1;
			right  = (buff[0]&2)==2;
			middle = (buff[0]&4)==4;
			if (!(buff[0]|buff[1]|buff[2])) {
				printf("released\n");
			}
			rx = buff[1];
			ry = buff[2];
			if (right) {
				printf("pressed right button @ %d,%d\n", loc.y, loc.x);
			}
			if (left) {
				printf("pressed left button @ %d,%d\n", loc.y, loc.x);
			}
			if (middle) {
				printf("pressed middle button @ %d,%d\n", loc.y, loc.x);
			}

			loc.y-=ry;
			loc.x+=rx;
			ApplyBitmap(jar, b, loc, green);
		}
	}
}

