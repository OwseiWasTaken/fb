// char size = 16, 8

#define CHARLEN 16
#define CHARLINELEN 8

// read chars from draw/font
uint8* ReadChars (int len) {
	int fontfd = open("./draw/font", O_RDWR);
	uint8 *chars = malloc(CHARLEN*len);
	chars = (unsigned char *)mmap(0,
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
			*(ch + i*CHARLEN + j ) = (*(from+ i) & (1<<j))^0;
			//fprintf(jar.log, "%d:%d -> %d\n", (1<<j), *(from+ i), *(ch + i));
		}
	}
	return ch;
}

bool* ExpandMappedChar(uint8* from, int index) {
	return ExpandChar((from+CHARLEN*index));
}

void DrawChar(struct fbjar jar, bool* chr, int y, int x) {
	char* location = GetFbPos(jar, y, x);
	bool* ch = chr;
	for (int i = 0 ; i<CHARLEN; i++) {
		for (int j = 0 ; j<CHARLINELEN ; j++) {
			*(location+0) = *(ch)*255;
			*(location+1) = *(ch)*255;
			*(location+2) = *(ch)*255;
			fprintf(jar.log, "%s-%d:%d\n",
			*(ch)!=0?"@":" ",
			i+y, x+j);
			location += jar.bpp;
			ch++;
		}
		location += jar.skip;
		location -= CHARLINELEN*jar.bpp;
		//ch += CHARLEN;
		fprintf(jar.log, "\n");
		//location += jar.skip;
	}
	//for (int i = 0 ; i<CHARLEN; i++) {
	//	for (uint j = 0 ; j<CHARLINELEN ; j++) {
	//		*(location + jar.bpp*i + jar.skip*j) = *(chr+i*CHARLINELEN+j);
	//	}
	//}
}

