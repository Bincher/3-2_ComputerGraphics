#include <iostream>
#include <gl/glut.h>

const int M = 500;
int image[M][M];
int mFlag = 0;

GLint TopLeftX, TopLeftY, BottomRightX, BottomRightY;

double currentXMin = -2.0;
double currentXMax = 2.0;
double currentYMin = -2.0;
double currentYMax = 2.0;

double juliaConstantReal = -0.7;
double juliaConstantImag = 0.27015;

void generateJuliaSet(double xMin, double xMax, double yMin, double yMax) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            double x = xMin + (double)j / M * (xMax - xMin);
            double y = yMin + (double)i / M * (yMax - yMin);

            double zx = x;
            double zy = y;

            int iteration = 0;
            while (iteration < 100 && zx * zx + zy * zy < 4.0) {
                double tmp = zx * zx - zy * zy + juliaConstantReal;
                zy = 2.0 * zx * zy + juliaConstantImag;
                zx = tmp;
                iteration++;
            }

            image[i][j] = iteration % 256;
        }
    }
}

void generateJuliaSetForViewport(double xMin, double xMax, double yMin, double yMax) {
    currentXMin = xMin;
    currentXMax = xMax;
    currentYMin = yMin;
    currentYMax = yMax;

    generateJuliaSet(xMin, xMax, yMin, yMax);
}

void MyDisplay() {
    glViewport(0, 0, 500, 500);
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_POINTS);
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            glColor3ub(image[i][j], image[i][j], image[i][j]);
            glVertex3f((float)j / M, (float)i / M, 0.0);
        }
    }
    glEnd();

    if (mFlag) {
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
        }
        else if (State == GLUT_UP) {
            BottomRightX = X;
            BottomRightY = Y;
            mFlag = 0;

            if (BottomRightX < TopLeftX) {
                std::swap(TopLeftX, BottomRightX);
            }
            if (BottomRightY < TopLeftY) {
                std::swap(TopLeftY, BottomRightY);
            }

            double xMin = (double)TopLeftX / M * (currentXMax - currentXMin) + currentXMin;
            double xMax = (double)BottomRightX / M * (currentXMax - currentXMin) + currentXMin;
            double yMin = currentYMax - (double)BottomRightY / M * (currentYMax - currentYMin);
            double yMax = currentYMax - (double)TopLeftY / M * (currentYMax - currentYMin);

            generateJuliaSetForViewport(xMin, xMax, yMin, yMax);

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
    glutCreateWindow("Julia Set");
    MyInit();
    glutDisplayFunc(MyDisplay);
    glutMouseFunc(MyMouseClick);
    glutMotionFunc(MyMouseMove);

    generateJuliaSet(currentXMin, currentXMax, currentYMin, currentYMax);
    glutPostRedisplay();

    glutMainLoop();
    return 0;
}
