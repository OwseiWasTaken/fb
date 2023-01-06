#include "buffy.h"

int main(int argc, char *argv[]) {
	struct fbjar jar = InitBuffy();
	printf("%d%s\n", argc, *argv);

	line l = MakeLine(
		MakePoint(100, 130),
		MakePoint(120, 160)
	);
	color pink = RGB(255,128,128);
	color hl = RGB(255,128,0);

	DrawLine(jar, l, 100, pink);
	l = RevExpandLine(l, 0.5);
	sleep(2);
	DrawLine(jar, l, 100, hl);

	StopBuffy(jar);
	return 0;
}
