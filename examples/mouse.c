#include "buffy.h"

// 8 = released
// 8+2 =
// 8+4 = middle

int main(int argc, char* argv[]){
	struct fbjar jar = InitBuffy();
	printf("%s%d\n", argv[0], argc);
	int mice = open("/dev/input/mice", O_RDONLY);
	assert(mice != -1);
	printf("reading mouse from fd%d\n", mice);

	bitmap b;
	b.heigth = 8;
	b.width = 8;
	uint8 cont[] = {
		0b11111000,
		0b11000000,
		0b11100000,
		0b10110000,
		0b10011000,
		0b00001100,
		0b00000110,
		0b00000011
	};
	PutBytesInBitmap(&b, cont);
	color green = RGB(0,255,0);
	color black = RGB(0,0,0);
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

			ApplyBitmap(jar, b, loc, black);
			loc.y-=ry;
			loc.x+=rx;
			ApplyBitmap(jar, b, loc, green);
		}
	}
}

