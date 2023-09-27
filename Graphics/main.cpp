#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <math.h>

#define H 480
#define W 640

unsigned char Iarray[H][W];
int linepatt[4][8] = { 1,1,1,1,1,1,1,1,//SOLID
						1,1,1,1,0,0,0,0,//DASHED
						1,0,1,0,1,0,1,0,//DOTTED
						1,1,1,0,1,1,1,0 };//User defined

enum { SOLID, DASHED, DOTTED, USER };


void lineDDA(int x1, int y1, int x2, int y2, int lt)
{
	int dx = x2 - x1, dy = y2 - y1, step, k;
	float xinc, yinc, x = (float)x1, y = (float)y1;

	if (abs(dx) > abs(dy)) step = abs(dx);
	else step = abs(dy);
	xinc = dx / (float)step;
	yinc = dy / (float)step;

	Iarray[x1][y1] = 0; // start pixel marking(setpixel function)
	for (k = 0; k < step; k++) {
		x += xinc;
		y += yinc;
		if (linepatt[lt][k % 8])
			Iarray[(int)(x + 0.5)][(int)(y + 0.5)] = 0;// marking(setpixel function)
	}
}

void main()
{
	memset(Iarray, 255, H * W);
	lineDDA(0, 0, H - 1, W - 1, SOLID);
	lineDDA(H - 1, 0, 0, W - 1, DASHED);
	lineDDA(10, 20, 300, 200, DOTTED);
	lineDDA(200, 100, 0, 500, USER);

	FILE* fp;
	fp = fopen("dda-lp.ppm", "wb");
	fprintf(fp, "P5\n");
	fprintf(fp, "%d %d\n", W, H); //file size 
	fprintf(fp, "255\n"); //max level 
	fwrite(Iarray, sizeof(char), H * W, fp);
	fclose(fp);
}