#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <math.h>

#define H 480
#define W 640

unsigned char Iarray[H][W][3];
int linepatt[4][8] = { 1,1,1,1,1,1,1,1,//SOLID
						1,1,1,1,0,0,0,0,//DASHED
						1,0,1,0,1,0,1,0,//DOTTED
						1,1,1,0,1,1,1,0 };//User defined

enum { SOLID, DASHED, DOTTED, USER };


void lineDDA(int x1, int y1, int x2, int y2, int lt, unsigned char r, unsigned char g, unsigned char b)
{
	int dx = x2 - x1, dy = y2 - y1, step, k;
	float xinc, yinc, x = (float)x1, y = (float)y1;

	if (abs(dx) > abs(dy)) step = abs(dx);
	else step = abs(dy);
	xinc = dx / (float)step;
	yinc = dy / (float)step;

	Iarray[x1][y1][0] = r; // start pixel marking(setpixel function)
	Iarray[x1][y1][1] = g;
	Iarray[x1][y1][2] = b;
	for (k = 0; k < step; k++) {
		x += xinc;
		y += yinc;
		if (linepatt[lt][k % 8]) {
			Iarray[(int)(x + 0.5)][(int)(y + 0.5)][0] = r;// marking(setpixel function)
			Iarray[(int)(x + 0.5)][(int)(y + 0.5)][1] = g;
			Iarray[(int)(x + 0.5)][(int)(y + 0.5)][2] = b;
		}

	}
}
void main()
{
	memset(Iarray, 255, H * W);
	lineDDA(0, 0, H - 1, W - 1, SOLID, 255, 0, 0);
	lineDDA(H - 1, 0, 0, W - 1, DASHED, 0, 255, 0);
	lineDDA(10, 20, 300, 200, DOTTED, 0, 0, 255);
	lineDDA(200, 100, 0, 500, USER, 255, 255, 0);

	FILE* fp;
	fp = fopen("dda-lp.ppm", "wb");
	fprintf(fp, "P6\n");
	fprintf(fp, "%d %d\n", W, H); //file size 
	fprintf(fp, "255\n"); //max level 
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			fwrite(&Iarray[i][j], sizeof(unsigned char), 3, fp); 
		}
	}
	fclose(fp);
}