#include "buffy.h"
#include <stdarg.h>
#include <sys/wait.h>

//acpi
//name id: status, %, h:m:s until statusEnd

typedef struct {
	int id;
	byte level;
} battery;

// count lineFeeds
int CountLF(char *lines) {
	int ret = 0;
	for (int i = 0; i<strlen(lines); i++) {
		ret += (lines[i] == '\n');
	}
	return ret;
}

int LineLen(char* line) {
	int ret;
	for (ret = 0; line[ret] != '\n'; ret++);
	return ret;
}

void SplitLines(char **lines, char* buff) {
	int sz = CountLF(buff);
	int off = 0;
	for (int i=0; i<sz; i++) {
		lines[i] = malloc(LineLen(buff+off));
		memcpy(lines[i], buff+off, LineLen(buff+off));
		off += LineLen(buff+off)+1;
	}
}

battery ProcLine(char *line) {
	line+=strlen("Battery ");
	int id = line[0]-'0';
	line++; // ID:
	while (line[0] != ':') {
		id+=(9*id)+line[0]-'0';
		line++;
	}
	line++;
	//status
	while (line[0] != ',') {
		line++;
	}
	line++;
	line++; //space
	byte lvl = 0;
	while (line[0] != '%') {
		lvl+=9*lvl+line[0]-'0';
		line++;
	}
	battery b = {.level = lvl, .id = id};
	return b;
}

void _Print_Bat(battery b) {
	printf("%i:%hhu%%\n", b.id, b.level);
}

void DrawBat(struct fbjar jar, int bat) {
	int padding = 2;
	int ylen = 30;
	int xlen = 100;
	int yoff = 30;
	int xoff = 30;
	point tp = MakePoint(yoff, xlen+xoff-bat);
	point btop = MakePoint(yoff-padding-1, xoff-padding);
	point bt = MakePoint(ylen+yoff,xlen+xoff);
	point bbot = MovePoint(bt, padding, padding);

	color ener = RGB(0,255,0);
	if ( bat < 30 ) {
		ener = RGB(255,0,0);
	} else if (bat >= 95) {
		ener = RGB(255,255,255);
	}

	DrawRectangle(jar, btop, bbot, RGB(255,255,255));
	FillRectangle(jar, tp, bt, ener);
}

int main(int argc, char* argv[]) {
	// get info
	system("/bin/acpi -b > /tmp/acpiOut");
	FILE *rd = fopen("/tmp/acpiOut", "r");
	long unsigned bsz = 0xA000; // 40kb
	char *AcpiInfo = malloc(bsz);
	// read multi-line
	int offset = getline(&AcpiInfo, &bsz, rd);
	{
		int lo = offset;
		while (lo > 0) {
			AcpiInfo+=lo;
			lo = getline(&AcpiInfo, &bsz, rd);
			offset += lo;
		}
	}
	AcpiInfo-=(offset+1);
	assert(offset < bsz);

	int ilns = CountLF(AcpiInfo);
	char *lines[ilns];
	battery batts[ilns];
	int alllvl = 0;
	int mean = 0;

	SplitLines(lines, AcpiInfo);
	for (int i=0; i<ilns; i++) {
		batts[i] = ProcLine(lines[i]);
		if (batts[i].level) {
			alllvl+=batts[i].level;
			mean++;
		}
	}
	alllvl = alllvl/mean;
	// get avg batt level

	// show info
	
	struct fbjar jar = InitBuffy();
	if (argc-1) {
		for (int i = 0; i<100; i++) {
			DrawBat(jar, i);
			usleep(30000);
		}
	} else {
		DrawBat(jar, alllvl);
	}

	StopBuffy(jar);
	return 0;
}
