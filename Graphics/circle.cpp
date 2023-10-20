/*
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <math.h>

#define H 480
#define W 640

unsigned char Iarray[H][W][3];

void setPixel(int x, int y, int r, int g, int b) {
    Iarray[x][y][0] = r;
    Iarray[x][y][1] = g;
    Iarray[x][y][2] = b;
}

void circlePlot(int Cx, int Cy, int x, int y, int r, int g, int b)
{
    setPixel(Cx + x, Cy + y, r, g, b);
    setPixel(Cx - x, Cy + y, r, g, b);
    setPixel(Cx + x, Cy - y, r, g, b);
    setPixel(Cx - x, Cy - y, r, g, b);
    setPixel(Cx + y, Cy + x, r, g, b);
    setPixel(Cx - y, Cy + x, r, g, b);
    setPixel(Cx + y, Cy - x, r, g, b);
    setPixel(Cx - y, Cy - x, r, g, b);
}

void circleMidpoint(int xCenter, int yCenter, int radius, int r, int g, int b)
{
    int x = 0, y = radius, p = 1 - radius;

    circlePlot(xCenter, yCenter, x, y, r, g, b);

    while (x <= y) {
        x++;
        if (p < 0) p += 2 * x + 1;
        else { y--; p += 2 * (x - y) + 1; }

        circlePlot(xCenter, yCenter, x, y, r, g, b);
    }
}

int main()
{
    memset(Iarray, 255, H * W);

    circleMidpoint(320, 240, 100, 0, 0, 255);
    circleMidpoint(300, 200, 50, 255, 0, 0);
    circleMidpoint(240, 200, 70, 0, 255, 0);
    circleMidpoint(160, 120, 30, 255, 0, 255);

    FILE* fp;
    fp = fopen("circle.ppm", "wb");
    fprintf(fp, "P6\n");
    fprintf(fp, "%d %d\n", W, H); // File size
    fprintf(fp, "255\n");         // Max color level

    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            fwrite(&Iarray[i][j], sizeof(unsigned char), 3, fp);
        }
    }

    fclose(fp);

    return 0;
}
*/