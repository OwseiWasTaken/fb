#include "buffy.h"

int main ( int argc, char** argv ) {
	struct fbjar jar = InitBuffy();

	CloseFb(jar);
	return 0;
}



