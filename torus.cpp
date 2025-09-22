#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <glut.h>
using namespace std;ooo

const double pi = 3.1415;
bool is_pressed = false;
double camera_phi = 0, camera_theta = 0, angle = 0;
int screen_w = 1000, screen_h = 500;

double r = 1, R = 5;
int slices = 20, stacks = 10;

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

void tor_point(double r, double R, double phi, double theta) {
    glNormal3d(
        (R + r * cos(theta)) * cos(phi),
        (R + r * cos(theta)) * sin(phi),
        r * sin(theta)
    );
    glVertex3d(
        (R + r * cos(theta)) * cos(phi),
        (R + r * cos(theta)) * sin(phi),
        r * sin(theta)
    );
}

void draw_tor(double r, double R, int slices, int stacks) {
    double dphi = 2 * pi / slices;
    double dtheta = pi / stacks;
    double theta = -pi / 2;

    for (int y = 0; y < stacks; y++, theta += dtheta) {
        double phi = 0;
        glBegin(GL_QUAD_STRIP);
        for (int x = 0; x <= slices; x++, phi += dphi) {
            tor_point(r, R, phi, theta);
            tor_point(r, R, phi, theta + dtheta);
        }
        glEnd();
        phi = 0;
        glBegin(GL_QUAD_STRIP);
        for (int x = 0; x <= slices; x++, phi += dphi) {
            tor_point(r, -R, phi, theta);
            tor_point(r, -R, phi, theta + dtheta);
        }
        glEnd();
    }
}

void display() {
    glClearColor(0.85, 0, 0.85, 0.9);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glColor3d(0, 1, 0);
    draw_tor(r, R, slices, stacks);

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
