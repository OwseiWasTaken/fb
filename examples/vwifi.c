#include "buffy.h"

int CountDecimalDigits( int v ) {
	int i;
	for (i = 0; v; i++) {v = v/10;}
	return i;
}

int GetDelay() {
	system("ping 8.8.8.8 -c 2 | sed 's/ /\\n/g' | sed 's/\\\\/\\n /g' | tail -n 2 | head -n 1 > /tmp/PingOut");
	FILE *rd = fopen("/tmp/PingOut", "r");

	while (getc(rd) != '/') {} // skip min, go to avg
	char savg[3];
	fgets(savg, 3, rd); // reads NN\0
	int avg = atoi(savg);
	assert(!(avg == 0)); // avg can't be 0ms

	fclose(rd);
	return avg;
}

int main (int argc, char *argv[]){
	int precision = 10;
	int AvgDelay = GetDelay(precision); // in ms
	// get "excelent", "good", "medium", "bad" delays
	// make wifi symbol bitmap
	printf("%i -> %i\n%i\n", 1000, CountDecimalDigits(1234), AvgDelay);
}

