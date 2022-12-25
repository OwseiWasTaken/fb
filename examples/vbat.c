#include "buffy.h"
#include <stdarg.h>
#include <sys/wait.h>

//acpi
//name id: status, %, h:m:s until statusEnd

typedef struct {
	int id;
	byte level;
} battery;

// move to cutil
ssize_t FindIndex(void *arr, ssize_t arrsize, byte cmp) {
	ssize_t i = 0;
	for (; i<arrsize; i++) {
		if (((byte*)arr)[i] == cmp) break;
	}
	if (i == arrsize) {return -1;}
	return i;
}

// move to cutil
// count lineFeeds
int CountLF(char *lines) {
	int ret = 0;
	for (int i = 0; i<strlen(lines); i++) {
		ret += (lines[i] == '\n');
	}
	return ret;
}

// move to cutil
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

void DrawBat(struct fbjar jar, int bat, int pad, point top, point len, color *colover) {
	// map bat level into len.x
	//int padding = 2;
	//float ratio = (float)len.x / (float)bat;
	float ratio = 100/(float)len.x;
	int lvl = (int)((float)bat/ratio);
	point tp = MovePoint(top, 0, len.x-lvl);
	point bt = MakePoint(len.y+top.y,len.x+top.x);
	point btop = MakePoint(top.y-pad-1, top.x-pad);
	point bbot = MovePoint(bt, pad, pad);

	color ener = RGB(0,255,0);
	if ( bat < 30 ) {
		ener = RGB(255,0,0);
	} else if (bat >= 95) {
		ener = RGB(255,255,255);
	}
	if (colover != NULL) {
		ener = *colover;
	}

	DrawRectangle(jar, btop, bbot, RGB(255,255,255));
	FillRectangle(jar, tp, bt, ener);
}

int GetAvgBat() { // get info and store avg batt% in alllvl
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
	fclose(rd);
	AcpiInfo-=(offset+1);
	assert(offset < bsz);
	int ilns = CountLF(AcpiInfo);
	char *lines[ilns];
	battery batts[ilns];

	int alllvl = 0;
	int count = 0;

	SplitLines(lines, AcpiInfo); // !mallocs lines
	free(AcpiInfo);
	for (int i=0; i<ilns; i++) {
		batts[i] = ProcLine(lines[i]);
		free(lines[i]);
		if (batts[i].level) {
			alllvl+=batts[i].level;
			count++;
		}
	}
	return alllvl/count;
}

int main(int argc, char* argv[]) {

	// show info
	struct fbjar jar = InitBuffy();

	bool animate = false;
	bool loop = false;
	int pad = 2;
	point top = MakePoint(30,30);
	point len = MakePoint(30, 100);

	for (int i = 0 ; i<argc ; i++) {
		if (!strcmp(argv[i], "-a")) {
			animate = true;
		} else if (!strcmp(argv[i], "--animate")) {
			animate = true;
		} else if (!strcmp(argv[i], "-p")) {
			i++;
			assert(i!=argc);
			pad = atoi(argv[i]);
			assert(!(pad == 0 && !(argv[i][0] == '0')));
		} else if (!strcmp(argv[i], "-t")) {
			i++;
			assert(i!=argc);
			ssize_t split = FindIndex(argv[i], strlen(argv[i]), ',');
			assert(split != -1);
			argv[i][split] = '\0';
			int y = atoi(argv[i]);
			int x = atoi(argv[i]+split+1);
			assert(!(y == 0 && !(argv[i][0] == '0')));
			assert(!(x == 0 && !(argv[i][split+1] == '0')));
			top = MakePoint(y, x);
		} else if (!strcmp(argv[i], "-l")) {
			i++;
			assert(i!=argc);
			ssize_t split = FindIndex(argv[i], strlen(argv[i]), ',');
			assert(split != -1);
			argv[i][split] = '\0';
			int y = atoi(argv[i]);
			int x = atoi(argv[i]+split+1);
			assert(!(y == 0 && !(argv[i][0] == '0')));
			assert(!(x == 0 && !(argv[i][split+1] == '0')));
			len = MakePoint(y, x);
		} else if (!strcmp(argv[i], "--loop")) {
			loop = true;
		}
	}

	assert(top.y-pad > 0);
	assert(top.x-pad > 0);
	int BatLvl = GetAvgBat();
	DrawBat(jar, BatLvl, pad, top, len, NULL);
	for (byte i = 0; loop; i++) {
		color clear = RGB(0,0,0);
		color charging = RGB(80,200,80);
		// for charing animation
		if (animate) {
			for (int i = 0; i<11; i++) {
				DrawBat(jar, i*10-1, pad, top, len, &charging);
				usleep(909090); // 10/11 sec
			}
			DrawBat(jar, 100, pad, top, len, &clear);
		} else {
			DrawBat(jar, BatLvl, pad, top, len, NULL);
		}
		sleep(1);
		if (!i%10) BatLvl = GetAvgBat();
	}

	StopBuffy(jar);
	return 0;
}
