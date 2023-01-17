void _P_point (point p);

void _P_line (line l);

void _P_triangle (triangle t);

void _P_quad (quad q);

void _P_ppoint (ppoint p);

void _P_pline (pline l);

void _P_ptriangle (ptriangle t);

void _P_pquad (pquad q);

struct fbjar InitFb();

struct fbjar InitMemJar ();

void CloseFb(struct fbjar jar);

void SHandleInt( int sig );

void StopBuffy(struct fbjar jar);

struct fbjar InitBuffy();

point MakePoint(const int y, const int x);

upoint MakeuPoint(const uint y, const uint x);

ppoint MakepPoint(const lfloat y, const lfloat x);

ppoint pMakepPoint(const point p);

point pMakePoint(const ppoint p);

line MakeLine(const point a, const point b);

pline MakepLine(const ppoint a, const ppoint b);

line pMakeLine(const ppoint a, const ppoint b);

polar MakePolar(const float r, const float a);

triangle MakeTriangle(const point a, const point b, const point c);

triangle MakeSTriangle(const point a, const point b, const point c);

point MovePoint(const point p, const int y, const int x);

point pMovePoint(const point p, const point d /*diff*/ );

ppoint pMovepPoint(const ppoint p, const point d /*diff*/ );

ppoint ppMovepPoint(const ppoint p, const ppoint d /*diff*/ );

line LineSwap(line l);

line ExpandLine(line l, lfloat b);

line RevExpandLine(line l, lfloat b);

point plMakePoint (polar plr);

ppoint plMakepPoint (polar plr);

point GetTriangleTop(triangle tri);

point GetTriangleBot(triangle tri);

point GetTriangleCont(triangle tri);

void SortTriangle(triangle tri, point *dest);

void SSortTriangle(triangle *tri);

void vApplyTriangle(triangle *tri, point *source);

point SPolarToCoord (polar plr);

ppoint PolarToCoord (polar plr);

point pMul(point p, int m);

ppoint ppMul(ppoint p, int m);

point pDiv(point p, int d);

point pAdd(point p, int a);

point pAddp(point p, point a);

point pAddpp(point p, ppoint a);

point pSub(point p, int s);

point pSubp(point p, point s);

point pSubpp(point p, ppoint s);

bool pEq(point a, point b);

bool pGt(point a, point b);

bool pSt(point a, point b);

lfloat lerp(lfloat a, lfloat b, lfloat p);

ppoint D1PointLerp(point a, point b, lfloat t);

ppoint D1pPointLerp(ppoint a, ppoint b, lfloat t);

ppoint D1LineLerp(line l, lfloat t);

ppoint D2pLineLerp(pline l, lfloat t);

float GetDistance(const point a, const point b);

lfloat pGetDistance(const ppoint a, const point b);

float GetADistance(const point a, const point b);

point GetpDistance(const point a, const point b);

point GetpADistance(const point a, const point b);

lfloat GetAngle(point reference, const point check);

long int GetPixelPos ( struct fbjar jar, int y, int x );

long int GetPixelPnt ( struct fbjar jar, point p );

uint8* GetFbPnt ( struct fbjar jar, point p );

uint8* GetFbPos ( struct fbjar jar, int y, int x );

bool PInP(point top, point bot, point check);

bool CheckPIJ (struct fbjar jar, point p);

bool CheckuPIJ (struct fbjar jar, upoint p);

bool qCheckPIJ (struct fbjar jar, point p);

bool qCheckuPIJ (struct fbjar jar, upoint p);

bool CheckInJar (struct fbjar jar, uint y, uint x);

void UpdateBuffer(struct fbjar jar, uint8* newbuff);

void UpdateJar(struct fbjar * restrict jar, struct fbjar * restrict newjar);

bool* BytesToCont(int heigth, int width, byte* Bcont);

void PutBytesInBitmap(bitmap *b, byte* Bcont);

uint8* ReadByteFb(struct fbjar jar, point top, point bot);

color* ReadBytesFb(struct fbjar jar, point top, point bot);

void ReadByteFbIntoBytemap(struct fbjar jar, point top, bytemap *b);

void ReadByteFbIntoBytesmap(struct fbjar jar, point top, bytesmap *b);

int  LoadBytemap(char *pathname, bytemap *b);
