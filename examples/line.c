#include "buffy.h"

int main(void) {
	struct fbjar jar = InitBuffy();

	line la = MakeLine(
		MakePoint(50,0), // top
		MakePoint(150,0) // control
	);
	line lb = MakeLine(
		MakePoint(150,0), // control
		MakePoint(150,100) // bot
	);
	line bz;

	for (float i = 0.0; i<101.0;i++) {
		bz = MakeLine(
			GetPointByLineSeg(la, i),
			GetPointByLineSeg(lb, i)
		);
		SDrawPoint( jar, GetPILSeg(bz, i) );
		SDrawPoint( jar, GetPILSeg(la, i) );
		SDrawPoint( jar, GetPILSeg(lb, i) );
	}

	StopBuffy(jar);
	return 0;
}
