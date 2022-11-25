// char size = 16, 8

#define CHARLEN 16
#define CHARLINELEN 8

// read chars from draw/font
uint8* ReadChars (int len) {
	int fontfd = open("./draw/font", O_RDWR);
	uint8 *chars = malloc(CHARLEN*len);
	chars = (uint8 *)mmap(0,
			CHARLEN*len, PROT_READ, MAP_SHARED, fontfd, 0);
	return chars;
}

//for (int i = 0 ; i<CHARLEN; i++) {
	//for (int j = 0 ; j<8 ; j++) {
	//	fprintf(jar.log, "%s", *(ch+i*CHARLEN+j)?"@":".");
	//}
//}

bool* ExpandChar(uint8* from) {
	bool *ch = malloc(CHARLEN);
	for (int i = 0 ; i<CHARLEN; i++) {
		for (uint j = 0 ; j<CHARLINELEN ; j++) {
			*(ch + i*CHARLEN + j ) = (*(from+ i) & (1<<j))!=0;
			//fprintf(jar.log, "%d:%d -> %d\n", (1<<j), *(from+ i), *(ch + i));
		}
	}
	return ch;
}

bool* ExpandMappedChar(uint8* from, int index) {
	return ExpandChar((from+CHARLEN*index));
}

void DrawChar(struct fbjar jar, uint8* chr, int y, int x) {
	uint8* location = GetFbPos(jar, y, x);
	for (int i = 0 ; i<CHARLEN; i++) {
		fprintf(jar.log, "\nLine: %d\n", i);
		for (uint j = 0 ; j<CHARLINELEN ; j++) {
			*(location+0) = ((*(chr + i) & (1<<j))!=0)*255;
			*(location+1) = ((*(chr + i) & (1<<j))!=0)*255;
			*(location+2) = ((*(chr + i) & (1<<j))!=0)*255;
			fprintf(jar.log, "%d&%d=%d->",
				*(chr + i),(1<<j),
				(*(chr+i)&(1<<j))!=0
			);
			fprintf(jar.log, "%d\n", *(location+1));
			// next pixel
			location += jar.bpp;
		}
		// next line
		location += jar.skip;
		location -= CHARLINELEN*jar.bpp;
	}
	return;
}
