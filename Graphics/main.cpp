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
                      1,1,1,0,1,1,1,0 }; // USER

enum { SOLID, DASHED, DOTTED, USER };

// DDA
void lineDDA(int x1, int y1, int x2, int y2, int lt, unsigned char r, unsigned char g, unsigned char b) {
    int dx = x2 - x1, dy = y2 - y1, step, k;
    float xinc, yinc, x = (float)x1, y = (float)y1;

    if (abs(dx) > abs(dy)) step = abs(dx);
    else step = abs(dy);
    xinc = dx / (float)step;
    yinc = dy / (float)step;

    Iarray[x1][y1][0] = r; // setpixel
    Iarray[x1][y1][1] = g;
    Iarray[x1][y1][2] = b;
    for (k = 0; k < step; k++) {
        x += xinc;
        y += yinc;
        if (linepatt[lt][k % 8]) {
            Iarray[(int)(x + 0.5)][(int)(y + 0.5)][0] = r; // 마킹
            Iarray[(int)(x + 0.5)][(int)(y + 0.5)][1] = g;
            Iarray[(int)(x + 0.5)][(int)(y + 0.5)][2] = b;
        }
    }
}

// 베지어 스플라인
void drawBezierSpline(int x[], int y[], int n, int r, int g, int b) {
    int C[6] = { 1, 5, 10, 10, 5, 1 };
    float t, u, xpt, ypt;
    int npts, k, j;
    int* bin;

    npts = 500;

    bin = new int[n + 1];

    // 이항 계수 C
    for (k = 0; k <= n; k++) {
        bin[k] = 1;
        for (j = n; j >= k + 1; j--)
            bin[k] *= j;
        for (j = n - k; j >= 2; j--)
            bin[k] /= j;
    }

    // 그리기
    for (k = 0; k < npts; k++) {
        t = (float)k / (npts - 1);
        u = 1 - t;
        xpt = 0;
        ypt = 0;

        for (j = 0; j <= n; j++) {
            xpt += bin[j] * pow(u, (float)(n - j)) * pow(t, (float)j) * x[j];
            ypt += bin[j] * pow(u, (float)(n - j)) * pow(t, (float)j) * y[j];
        }

        if (k > 0) {
            lineDDA((int)xpt, (int)ypt, (int)xpt, (int)ypt, SOLID, r, g, b);
        }
    }

    delete[] bin;
    printf("그리기 종료\n");
}

int main() {
    printf("시작\n");

    int numControlPoints = 5;
    int x[5] = { 50, 150, 250, 350, 450 };
    int y[5] = { 150, 450, 50, 400, 100 };

    drawBezierSpline(x, y, numControlPoints - 1, 255, 255, 255);

    printf("파일 생성 시작\n");
    FILE* fp;
    fp = fopen("bezier_spline.ppm", "wb");
    fprintf(fp, "P6\n");
    fprintf(fp, "%d %d\n", W, H); // 파일 크기
    fprintf(fp, "255\n");         // 최대 레벨
    for (int i = 0; i < numControlPoints - 1; i++) {
        int x1 = x[i];
        int y1 = y[i];
        int x2 = x[i+1];
        int y2 = y[i+1];
        lineDDA(x1, y1, x2, y2, DASHED, 255, 255, 255);
    }
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            fwrite(&Iarray[i][j], sizeof(unsigned char), 3, fp);
        }
    }
    fclose(fp);
    printf("완료\n");
    return 0;
}