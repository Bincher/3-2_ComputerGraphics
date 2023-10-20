#include <stdio.h>
#include <string.h>
#include <math.h>
#pragma warning(disable: 4996)
#define H 680
#define W 840

unsigned char Iarray[H][W][3];
int linepatt[4][8] = { 1,1,1,1,1,1,1,1, // SOLID
                      1,1,1,1,0,0,0,0, // DASHED
                      1,0,1,0,1,0,1,0, // DOTTED
                      1,1,1,0,1,1,1,0 }; // USER;

enum { SOLID, DASHED, DOTTED, USER };

// DDA 알고리즘을 이용한 선 그리기 함수
void lineDDA(int x1, int y1, int x2, int y2, int lt, unsigned char r, unsigned char g, unsigned char b) {
    int dx = x2 - x1, dy = y2 - y1, step, k;
    float xinc, yinc, x = (float)x1, y = (float)y1;

    if (abs(dx) > abs(dy)) step = abs(dx);
    else step = abs(dy);
    xinc = dx / (float)step;
    yinc = dy / (float)step;

    Iarray[x1][y1][0] = r; // 시작 픽셀 마킹 (setpixel 함수)
    Iarray[x1][y1][1] = g;
    Iarray[x1][y1][2] = b;
    for (k = 0; k < step; k++) {
        x += xinc;
        y += yinc;
        if (linepatt[lt][k % 8]) {
            Iarray[(int)(x + 0.5)][(int)(y + 0.5)][0] = r; // 마킹 (setpixel 함수)
            Iarray[(int)(x + 0.5)][(int)(y + 0.5)][1] = g;
            Iarray[(int)(x + 0.5)][(int)(y + 0.5)][2] = b;
        }
    }
}

// Koch 곡선 그리기 함수
void kochCurve(int x1, int y1, int x5, int y5, int depth, int lt, unsigned char r, unsigned char g, unsigned char b) {
    int x2, y2, x3, y3, x4, y4;
    int dx, dy;
    if (depth == 0) {
        lineDDA(x1, y1, x5, y5, lt, r, g, b);
    }
    else {

        dx = (x5 - x1) / 3;
        dy = (y5 - y1) / 3;
        x2 = x1 + dx;
        y2 = y1 + dy;
        x3 = (int)(0.5 * (x1 + x5) + sqrt(3) * (y1 - y5) / 6);
        y3 = (int)(0.5 * (y1 + y5) + sqrt(3) * (x5 - x1) / 6);
        x4 = 2 * dx + x1;
        y4 = 2 * dy + y1;

        kochCurve(x1, y1, x2, y2, depth - 1, lt, r, g, b);
        kochCurve(x2, y2, x3, y3, depth - 1, lt, r, g, b);
        kochCurve(x3, y3, x4, y4, depth - 1, lt, r, g, b);
        kochCurve(x4, y4, x5, y5, depth - 1, lt, r, g, b);
    }
}

int main() {
    printf("시작\n");
    kochCurve(280, 280, 150, 20, 5, SOLID, 255, 255, 255);
    kochCurve(150 , 20, 20, 280, 5, SOLID, 255, 255, 255);
    kochCurve(20, 280, 280, 280, 5, SOLID, 255, 255, 255);
    
   
    printf("파일 생성 시작\n");
    FILE* fp;
    fp = fopen("koch_curve.ppm", "wb");
    fprintf(fp, "P6\n");
    fprintf(fp, "%d %d\n", W, H); // 파일 크기
    fprintf(fp, "255\n");         // 최대 레벨

    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            fwrite(&Iarray[i][j], sizeof(unsigned char), 3, fp);
        }
    }
    fclose(fp);
    printf("완료\n");
    return 0;
}
