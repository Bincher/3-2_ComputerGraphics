#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <math.h>

#define H 480
#define W 640

//unsigned char Iarray[H][W];
unsigned char Iarray[H][W][3];

void lineBres(int x1, int y1, int x2, int y2, unsigned char r, unsigned char g, unsigned char b)
{
    int dx = abs(x2 - x1), dy = abs(y2 - y1);
    int p, twoDy, twoDyDx;
    int x, y, xEnd, yEnd, IorD;

    if (dx > dy) {
        p = 2 * dy - dx; twoDy = 2 * dy; twoDyDx = 2 * (dy - dx);

        if (x1 > x2) { 
            x = x2; 
            y = y2; 
            xEnd = x1; 
            if (y1 - y2 > 0)IorD = 1; 
            else IorD = -1; 
        }
        else { 
            x = x1; 
            y = y1; 
            xEnd = x2; 
            if (y2 - y1 > 0)IorD = 1; 
            else IorD = -1; 
        }

        Iarray[x][y][0] = r; 
        Iarray[x][y][1] = g; 
        Iarray[x][y][2] = b;// start point marking with a user-defined color(r,g,b) 

        while (x < xEnd) {
            x++;
            if (p < 0) p += twoDy;
            else { 
                y += IorD; 
                p += twoDyDx; 
            }
            Iarray[x][y][0] = r; 
            Iarray[x][y][1] = g; 
            Iarray[x][y][2] = b;//marking
        }// end of while
    }// end of if
    else {
        p = 2 * dx - dy;
        twoDy = 2 * dx;
        twoDyDx = 2 * (dx - dy);

        if (y1 > y2) {
            x = x2;
            y = y2;
            yEnd = y1;
            if (x1 - x2 > 0) IorD = 1;
            else IorD = -1;
        }
        else {
            x = x1;
            y = y1;
            yEnd = y2;
            if (x2 - x1 > 0) IorD = 1;
            else IorD = -1;
        }

        Iarray[x][y][0] = r;
        Iarray[x][y][1] = g;
        Iarray[x][y][2] = b; // 시작점 표시

        while (y < yEnd) {
            y++;
            if (p < 0) p += twoDy;
            else {
                x += IorD;
                p += twoDyDx;
            }
            Iarray[x][y][0] = r;
            Iarray[x][y][1] = g;
            Iarray[x][y][2] = b; // 해당 픽셀 색칠
        }
    }// end of else

}


void main()
{
    memset(Iarray, 255, H * W);
    memset(Iarray, 255, H * W);
    memset(Iarray, 255, H * W);

    lineBres(0, 0, H - 1, W - 1, 255, 0, 0);
    lineBres(H - 1, 0, 0, W - 1, 0, 255, 0);
    lineBres(10, 20, 300, 200, 0, 0, 255);
    lineBres(200, 100, 0, 500, 255, 255, 0);

    FILE* fp;
    fp = fopen("bres-lp.ppm", "wb");
    fprintf(fp, "P6\n");
    fprintf(fp, "%d %d\n", W, H); // file size
    fprintf(fp, "255\n"); // max level

    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            fwrite(&Iarray[i][j], sizeof(unsigned char), 1, fp);
            fwrite(&Iarray[i][j], sizeof(unsigned char), 1, fp);
            fwrite(&Iarray[i][j], sizeof(unsigned char), 1, fp);
        }
    }

    fclose(fp);
}