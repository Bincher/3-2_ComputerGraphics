#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <cmath>
#include <gl/glut.h>

const int M = 500;
int image[M][M];
int mFlag = 0;

GLint TopLeftX, TopLeftY, BottomRightX, BottomRightY;

void generateMandelbrot(double xMin, double xMax, double yMin, double yMax) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            double x = xMin + (double)j / M * (xMax - xMin);
            double y = yMin + (double)i / M * (yMax - yMin);

            double zx = 0, zy = 0;
            double cx = x, cy = y;

            int iteration = 0;
            while (iteration < 100 && zx * zx + zy * zy < 4.0) {
                double tmp = zx * zx - zy * zy + cx;
                zy = 2.0 * zx * zy + cy;
                zx = tmp;
                iteration++;
            }

            // 색상값 설정 (iteration 값 활용)
            image[i][j] = iteration % 256;
        }
    }
}

void generateMandelbrotForViewport() {
    double xMin = (double)TopLeftX / M * 3.5 - 2.5;
    double xMax = (double)BottomRightX / M * 3.5 - 2.5;
    double yMin = (double)TopLeftY / M * 2.0 - 1.0;
    double yMax = (double)BottomRightY / M * 2.0 - 1.0;
    generateMandelbrot(xMin, xMax, yMin, yMax);
}

void MyDisplay() {
    glViewport(0, 0, 500, 500);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the Mandelbrot
    glBegin(GL_POINTS);
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            glColor3ub(image[i][j], image[i][j], image[i][j]);
            glVertex3f((float)j / M, (float)i / M, 0.0);
        }
    }
    glEnd();

    if (mFlag) {
        // During dragging, draw the red border to represent the selected region
        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_LINE_LOOP);
        glVertex3f(TopLeftX / 500.0, (500 - TopLeftY) / 500.0, 0.0);
        glVertex3f(TopLeftX / 500.0, (500 - BottomRightY) / 500.0, 0.0);
        glVertex3f(BottomRightX / 500.0, (500 - BottomRightY) / 500.0, 0.0);
        glVertex3f(BottomRightX / 500.0, (500 - TopLeftY) / 500.0, 0.0);
        glEnd();
    }

    glutSwapBuffers();
}

void MyMouseClick(GLint Button, GLint State, GLint X, GLint Y) {
    if (Button == GLUT_LEFT_BUTTON) {
        if (State == GLUT_DOWN) {
            TopLeftX = X;
            TopLeftY = Y;
            mFlag = 1;
            printf("시작 좌표 : %d %d\n", X, Y);
        }
        else if (State == GLUT_UP) {
            BottomRightX = X;
            BottomRightY = Y;
            mFlag = 0;

            // Generate Mandelbrot based on the selected region
            generateMandelbrotForViewport();

            printf("끝 좌표 : %d %d\n", X, Y);
            glutPostRedisplay();
        }
    }
}

void MyMouseMove(GLint X, GLint Y) {
    BottomRightX = X;
    BottomRightY = Y;
    glutPostRedisplay();
}

void MyInit() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Mandelbrot Explorer");
    MyInit();
    glutDisplayFunc(MyDisplay);
    glutMouseFunc(MyMouseClick);
    glutMotionFunc(MyMouseMove);

    // Initial display of the Mandelbrot set
    generateMandelbrot(-2.5, 1.0, -1.0, 1.0);
    glutPostRedisplay();

    glutMainLoop();
    return 0;
}
