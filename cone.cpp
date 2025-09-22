#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <glut.h>
using namespace std;

const double pi = 3.1415;
bool is_pressed = false;
double camera_phi = 0, camera_theta = 0, angle = 0;
int screen_w = 1000, screen_h = 500;

double r = 1, h = 5;
int slices = 20;

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
}

void idle() {

}

void reshape(int wp, int hp) {
    glViewport(0, 0, wp, hp);
    screen_w = wp;
    screen_h = hp;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // CCS --> PCS
    gluPerspective(60, (double)wp / hp, 1, 100);

    // WCS --> CCS
    gluLookAt(
        10 * cos(camera_theta) * cos(camera_phi),
        10 * sin(camera_theta),
        10 * cos(camera_theta) * sin(camera_phi),
        0, 0, 0, 0, 1, 0);
    glRotated(-45, 1, 0, 0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotated(45, 1, 0, 0);
}

void cone_point(double r, double phi, double h) {
    glNormal3d(
        cos(phi),
        sin(phi),
        1
    );
    glVertex3d(
        r * cos(phi),
        r * sin(phi),
        h
    );
}

void draw_cone(double r, int slices, double h) {
    double dphi = 2 * pi / slices;
    double phi = 0;
    glBegin(GL_TRIANGLES);
    for (int x = 0; x < slices; x++, phi += dphi) {
        cone_point(r, phi, 0);
        cone_point(r, phi + dphi, 0);
        glNormal3d(
            0,
            0,
            1
        );
        glVertex3d(
            0,
            0,
            h
        );
    }
    glEnd();
}

void display() {
    glClearColor(0.85, 0, 0.85, 0.9);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glColor3d(0, 1, 0);
    draw_cone(r, slices, h);

    glFinish();

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {

}

void mouse_pressed(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        is_pressed = (state == GLUT_DOWN);
    }
}
void mouse_motion(int x, int y) {
    static int lastx = -1, lasty = -1;

    if (is_pressed) {
        camera_phi += 2 * pi * (x - lastx) / screen_w;
        camera_theta += pi * (y - lasty) / screen_h;
        camera_theta = max(min(camera_theta, pi / 2), -pi / 2);
        reshape(screen_w, screen_h);
    }

    lastx = x;
    lasty = y;
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitWindowPosition(3, 3);
    glutInitWindowSize(screen_w, screen_h);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);

    glutCreateWindow("mew^^");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMouseFunc(mouse_pressed);
    glutMotionFunc(mouse_motion);
    glutPassiveMotionFunc(mouse_motion);

    init();

    glutMainLoop();
}
