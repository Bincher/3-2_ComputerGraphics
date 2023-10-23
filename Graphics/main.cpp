#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#pragma warning(disable: 4996)
#define H 800
#define W 1600
#define M_PI 3.14159265358979323846

unsigned char Iarray[H][W][3];

void lineDDA(int x1, int y1, int x2, int y2, unsigned char r, unsigned char g, unsigned char b) {
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

void fractalTree(int x1, int y1, int length, int angle, int depth, unsigned char r, unsigned char g, unsigned char b) {
    if (depth == 0) {
        return;
    }

    int x2 = x1 + (int)(length * sin(angle * M_PI / 180));
    int y2 = y1 - (int)(length * cos(angle * M_PI / 180));
    lineDDA(x1, y1, x2, y2, r, g, b);

    int angle1 = angle + 30;
    int length1 = (int)(length * 0.7);
    fractalTree(x2, y2, length1, angle1, depth - 1, r, g, b);

    int angle2 = angle - 30;
    int length2 = (int)(length * 0.7);
    fractalTree(x2, y2, length2, angle2, depth - 1, r, g, b);
}
void fractalTreeRandom(int x1, int y1, int length, int angle, int depth, unsigned char r, unsigned char g, unsigned char b) {
    if (depth == 0) {
        return;
    }

    int x2 = x1 + (int)(length * sin(angle * M_PI / 180));
    int y2 = y1 - (int)(length * cos(angle * M_PI / 180));
    lineDDA(x1, y1, x2, y2, r, g, b);

    int angle1 = angle + (rand() % 60) - 30; // -30도에서 30도 사이 랜덤한 각도
    int length1 = (int)(length * (0.5 + ((double)rand() / RAND_MAX) * 0.5)); // 50%에서 100% 사이의 길이
    fractalTreeRandom(x2, y2, length1, angle1, depth - 1, r, g, b);

    int angle2 = angle + (rand() % 60) - 30; // -30도에서 30도 사이 랜덤한 각도
    int length2 = (int)(length * (0.5 + ((double)rand() / RAND_MAX) * 0.5)); // 50%에서 100% 사이의 길이
    fractalTreeRandom(x2, y2, length2, angle2, depth - 1, r, g, b);

}

int main() {
    printf("시작\n");

    int x1 = 100, y1 = 800;
    int length = 150;
    int angle = 90;

    srand((unsigned)time(NULL));

    fractalTree(x1, y1, length, angle, 7, 255, 255, 255); 
    printf("대칭형 트리 생성 완료\n");

    printf("대칭형 트리 파일 생성 시작\n");
    FILE* fp;
    fp = fopen("fractal_tree_symmetric.ppm", "wb");
    fprintf(fp, "P6\n");
    fprintf(fp, "%d %d\n", W, H);
    fprintf(fp, "255\n");

    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            fwrite(&Iarray[i][j], sizeof(unsigned char), 3, fp);
        }
    }
    fclose(fp);

    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            Iarray[i][j][0] = 0;
            Iarray[i][j][1] = 0;
            Iarray[i][j][2] = 0;
        }
    }

    x1 = 100, y1 = 800;
    length = 150;
    angle = 90;

    fractalTreeRandom(x1, y1, length, angle, 7, 255, 255, 255);
    printf("랜덤형 트리 생성 완료\n");

    printf("랜덤형 트리 파일 생성 시작\n");
    fp = fopen("fractal_tree_symmetric_random.ppm", "wb");
    fprintf(fp, "P6\n");
    fprintf(fp, "%d %d\n", W, H);
    fprintf(fp, "255\n");

    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            fwrite(&Iarray[i][j], sizeof(unsigned char), 3, fp);
        }
    }
    fclose(fp);

    return 0;
}
