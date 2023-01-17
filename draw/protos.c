void SDrawAllLine (struct fbjar jar, uint y);

void SDrawAllCollum (struct fbjar jar, uint x);

void SDrawPartLine (struct fbjar jar, uint y, uint StartX, uint EndX);

void SDrawPartCollum (struct fbjar jar, uint x, uint StartY, uint EndY);

void SDrawSquare (struct fbjar jar, point top, point bot);

void SRFillRectangle (struct fbjar jar, point top, point bot);

void SFillRectangle (struct fbjar jar, point top, point bot);

void SDrawPolarLine(struct fbjar jar, point top, polar bot);

void SDrawCircle (struct fbjar jar, point top, int r);

void SFillCircle (struct fbjar jar, point top, int r);

//unsanfe
void SDrawBitmap (struct fbjar jar, bitmap bmap, point top);

//unsanfe
void SApplyBitmap (struct fbjar jar, bitmap bmap, point top);

//unsanfe
void SDrawBytemap (struct fbjar jar, bytemap bmap, point top);

//unsanfe
void SApplyBytemap (struct fbjar jar, bytemap bmap, point top);

void SDrawPoint (struct fbjar jar, point at);

void SDrawpPoint (struct fbjar jar, ppoint at);

// precision = how many pixels (+1)
void SDrawLine (struct fbjar jar, line l, lfloat p);

void SDrawTiangle (struct fbjar jar, triangle tri, lfloat p);

void DrawAllLine (struct fbjar jar, uint y, color RGB);

void DrawAllCollum (struct fbjar jar, uint x, color RGB);

void DrawPartLine (struct fbjar jar, uint y, uint StartX, uint End, color RGB);

void DrawPartCollum (struct fbjar jar, uint x, uint StartY, uint EndY, color RGB);

void DrawRectangle(struct fbjar jar, point top, point bot, color RGB);

// R for relative
void RFillRectangle (struct fbjar jar, point top, point bot, color RGB);

void FillRectangle (struct fbjar jar, point top, point bot, color RGB);

//unsafe
void DrawPolarLine(struct fbjar jar, point top, polar bot, color RGB);

void DrawCircle (struct fbjar jar, point top, int r, color RGB);

void FillCircle (struct fbjar jar, point top, int r, color RGB);

void DrawBitmap (struct fbjar jar, bitmap bmap, point top, color RGB);

void ApplyBitmap (struct fbjar jar, bitmap bmap, point top, color RGB);

bytesmap SaveBytes (struct fbjar jar, point top, point bot);

bytesmap RSaveBytes (struct fbjar jar, point top, upoint len);

void DrawBytesmap (struct fbjar jar, bytesmap map, point top);

void ApplyBytesmap (struct fbjar jar, bytesmap map, point top);

// RGB
void DrawBytemap (struct fbjar jar, bytemap bmap, point top, uint chan);

void ApplyBytemap (struct fbjar jar, bytemap bmap, point top, uint chan);

void DrawPoint (struct fbjar jar, point at, color col);

void DrawpPoint (struct fbjar jar, ppoint at, color col);

// precision = how many pixels will be drawn (p+1)
void DrawLine (struct fbjar jar, line l, lfloat p, color col);

void DrawTiangle (struct fbjar jar, triangle tri, lfloat p, color col);

void TDrawPartLine ( struct fbjar jar, int y, int StartX, int EndX, color col, int thick);

void TDrawPartCollum ( struct fbjar jar, int x, int StartY, int EndY, color col, int thick);

//unsafe
// could do PIJ(R+/-top)
// but there are safe cases this would block
void TDrawPolarLine( struct fbjar jar, point top, polar bot, color col, int thick);

void TDrawPoint ( struct fbjar jar, point at, color col, int thick);

void TDrawpPoint ( struct fbjar jar, ppoint at, color col, int thick);

void TDrawLine ( struct fbjar jar, line l, lfloat p, color col, int thick);

void TDrawTiangle ( struct fbjar jar, triangle tri, lfloat p, color col, int thick);

