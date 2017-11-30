typedef unsigned char  byte;
typedef unsigned short word;
typedef unsigned long  dword;
typedef unsigned short ushort;
typedef unsigned long  ulong;

typedef struct RGB
{
	byte r, g, b;
} RGB;

typedef struct RGBpixmap
{
	int nRows, nCols;
	RGB *pixel;
} RGBpixmap;

void fskip(FILE *fp, int num_bytes);
ushort getShort(FILE *fp);
ulong getLong(FILE *fp);
void readBMPFile(RGBpixmap *pm, char *file);
void setTexture(RGBpixmap *p, GLuint textureID);