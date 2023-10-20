#include <stdio.h>
#include <string.h>
#include <math.h>
#pragma warning(disable: 4996)
#define H 800
#define W 1000

unsigned char Iarray[H][W][3];

enum { SOLID, DASHED, DOTTED, USER };

// DDA 알고리즘을 이용한 선 그리기 함수
void lineDDA(int x1, int y1, int x2, int y2, int lt, unsigned char r, unsigned char g, unsigned char b) {
    int dx = x2 - x1, dy = y2 - y1, step, k;
    float xinc, yinc, x = (float)x1, y = (float)y1;

    if (abs(dx) > abs(dy)) step = abs(dx);
    else step = abs(dy);
    xinc = dx / (float)step;
    yinc = dy / (float)step;

    Iarray[x1][y1][0] = r;
    Iarray[x1][y1][1] = g;
    Iarray[x1][y1][2] = b;
    for (k = 0; k < step; k++) {
        x += xinc;
        y += yinc;
        Iarray[(int)(x + 0.5)][(int)(y + 0.5)][0] = r;
        Iarray[(int)(x + 0.5)][(int)(y + 0.5)][1] = g;
        Iarray[(int)(x + 0.5)][(int)(y + 0.5)][2] = b;
    }
}

// Sierpinski Triangle
void sierpinski(int x1, int y1, int x2, int y2, int x3, int y3, int depth, int lt, unsigned char r, unsigned char g, unsigned char b) {
    if (depth == 0) {
        lineDDA(x1, y1, x2, y2, lt, r, g, b);
        lineDDA(x2, y2, x3, y3, lt, r, g, b);
        lineDDA(x3, y3, x1, y1, lt, r, g, b);
    }
    else {
        // 삼각형 중간 지점 
        int mid1x = (x1 + x2) / 2;
        int mid1y = (y1 + y2) / 2;
        int mid2x = (x2 + x3) / 2;
        int mid2y = (y2 + y3) / 2;
        int mid3x = (x1 + x3) / 2;
        int mid3y = (y1 + y3) / 2;

        // 재귀적으로 작은 삼각형을 그린다
        sierpinski(x1, y1, mid1x, mid1y, mid3x, mid3y, depth - 1, lt, r, g, b);
        sierpinski(mid1x, mid1y, x2, y2, mid2x, mid2y, depth - 1, lt, r, g, b);
        sierpinski(mid3x, mid3y, mid2x, mid2y, x3, y3, depth - 1, lt, r, g, b);
    }
}

int main() {
    printf("시작\n");

    // 큰 삼각형을 그리기 위한 좌표 설정
    int x1 = 420, y1 = 100;
    int x2 = 100, y2 = 580;
    int x3 = 740, y3 = 580;

    sierpinski(x1, y1, x2, y2, x3, y3, 4, SOLID, 255, 255, 255);

    printf("파일 생성 시작\n");
    FILE* fp;
    fp = fopen("sierpinski_triangle.ppm", "wb");
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
