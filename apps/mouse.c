#include "buffy.h"

// 8 = released
// 8+2 =
// 8+4 = middle

int main(int argc, char* argv[]){
	struct fbjar jar = InitBuffy();
	int mice = open("/dev/input/mice", O_RDONLY);
	assert(mice != -1);
	printf("reading mouse from fd%d\n", mice);

	point loc = MakePoint(100, 100);
	polar PointerM = MakePolar(12, 45);
	color black = RGB(0,0,0);

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
			//read and save pixels from fbmem, then rewrite
			DrawPolarLine(jar, loc, PointerM, black);
			DrawPartLine(jar, loc.y, loc.x, loc.x+4, black);
			DrawPartCollum(jar, loc.x, loc.y, loc.y+3, black);
			loc.y-=ry;
			loc.x+=rx;
			SDrawPolarLine(jar, loc, PointerM);
			SDrawPartLine(jar, loc.y, loc.x, loc.x+4);
			SDrawPartCollum(jar, loc.x, loc.y, loc.y+3);
			//printf("y:%d, x:%d\n", loc.y, loc.x);
		}
	}
}
