#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <math.h>

#define H 480
#define W 640

//unsigned char Iarray[H][W];
unsigned char Rarray[H][W];
unsigned char Garray[H][W];
unsigned char Barray[H][W];
int linepatt[4][8] = { 1,1,1,1,1,1,1,1,//SOLID
						1,1,1,1,0,0,0,0,//DASHED
						1,0,1,0,1,0,1,0,//DOTTED
						1,1,1,0,1,1,1,0 };//User defined

enum { SOLID, DASHED, DOTTED, USER };

void setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b) {
    Rarray[x][y] = r;
    Garray[x][y] = g;
    Barray[x][y] = b;
}

void lineDDA(int x1, int y1, int x2, int y2, int lt, unsigned char r, unsigned char g, unsigned char b)
{
    int dx = x2 - x1, dy = y2 - y1, step, k;
    float xinc, yinc, x = (float)x1, y = (float)y1;

    if (abs(dx) > abs(dy)) step = abs(dx);
    else step = abs(dy);
    xinc = dx / (float)step;
    yinc = dy / (float)step;

    setPixel(x1, y1, r, g, b); // start pixel marking(setPixel function)
    for (k = 0; k < step; k++) {
        x += xinc;
        y += yinc;
        if (linepatt[lt][k % 8])
            setPixel((int)(x + 0.5), (int)(y + 0.5), r, g, b); // marking(setPixel function)
    }
}

void main()
{
    memset(Rarray, 255, H * W);
    memset(Garray, 255, H * W);
    memset(Barray, 255, H * W);

    lineDDA(0, 0, H - 1, W - 1, SOLID, 255, 0, 0); // Red SOLID line
    lineDDA(H - 1, 0, 0, W - 1, DASHED, 0, 255, 0); // Green DASHED line
    lineDDA(10, 20, 300, 200, DOTTED, 0, 0, 255); // Blue DOTTED line
    lineDDA(200, 100, 0, 500, USER, 255, 255, 0); // Yellow USER line

    FILE* fp;
    fp = fopen("dda-lp.ppm", "wb");
    fprintf(fp, "P6\n");
    fprintf(fp, "%d %d\n", W, H); // file size
    fprintf(fp, "255\n"); // max level

    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            fwrite(&Rarray[i][j], sizeof(unsigned char), 1, fp);
            fwrite(&Garray[i][j], sizeof(unsigned char), 1, fp);
            fwrite(&Barray[i][j], sizeof(unsigned char), 1, fp);
        }
    }

    fclose(fp);
}