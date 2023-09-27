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
void ellipsePlot(int Cx, int Cy, int x, int y, int r, int g, int b)
{
    setPixel(Cx + x, Cy + y, r, g, b);//marking function according to the output device
    setPixel(Cx - x, Cy + y, r, g, b);
    setPixel(Cx + x, Cy - y, r, g, b);
    setPixel(Cx - x, Cy - y, r, g, b);
}
void ellipseMidpoint(int xCenter, int yCenter, int Rx, int Ry, int r, int g, int b)
{
    int Rx2 = Rx * Rx, Ry2 = Ry * Ry, twoRx2 = 2 * Rx2, twoRy2 = 2 * Ry2;
    int p, x = 0, y = Ry, px = 0, py = twoRx2 * y;

    ellipsePlot(xCenter, yCenter, x, y, r, g, b);//start pixel marking..

    //Region 1
    p = round(Ry2 - (Rx2 * Ry) + (0.25 * Rx2));
    while (px < py) {
        x++;
        px += twoRy2;
        if (p < 0) p += Ry2 + px;
        else { y--; py -= twoRx2; p += Ry2 + px - py; }

        ellipsePlot(xCenter, yCenter, x, y, r, g, b);//marking..
    }

    //Region 2
    p = round(Ry2 * (x + 0.5) * (x + 0.5) + Rx2 * (y - 1) * (y - 1) - Rx2 * Ry2);
    while (y > 0) {
        y--;
        py -= twoRx2;
        if (p > 0) p += Rx2 - py;
        else { x++; px += twoRy2; p += Rx2 - py + px; }

        ellipsePlot(xCenter, yCenter, x, y, r, g, b);//marking..
    }
}

int main()
{
    memset(Iarray, 255, H * W);

    ellipseMidpoint(320, 240, 100, 10, 0, 0, 255);
    ellipseMidpoint(300, 200, 50, 100, 255, 0, 0);
    ellipseMidpoint(240, 200, 70, 100, 0, 255, 0);
    ellipseMidpoint(160, 120, 30, 100, 255, 0, 255);

    FILE* fp;
    fp = fopen("ellipse.ppm", "wb");
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
