#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <glut.h>
using namespace std;

const double pi = 3.1415;
bool is_pressed = false;
double camera_phi = 0, camera_theta = 0, camera_angle = 0;
int screen_w = 1000, screen_h = 500;

double cone_r = 1, cone_h = 5, cone_angle = 0;
int cone_slices = 4;
double sphere_r = 1, sphere_angle = 0;
int sphere_slices = 10, sphere_stacks = 10;

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
        20 * cos(camera_theta) * cos(camera_phi),
        20 * sin(camera_theta),
        20 * cos(camera_theta) * sin(camera_phi),
        0, 0, 0, 0, 1, 0);
    glRotated(-45, 1, 0, 0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotated(45, 1, 0, 0);
}

void sphere_point(double sphere_r, double phi, double theta) {
    glNormal3d(
        cos(phi) * cos(theta),
        sin(phi) * cos(theta),
        sin(theta)
    );
    glVertex3d(
        sphere_r * cos(phi) * cos(theta),
        sphere_r * sin(phi) * cos(theta),
        sphere_r * sin(theta)
    );
}
void draw_sphere(double sphere_r, int sphere_stacks, int sphere_slices) {
    double dphi = 2 * pi / sphere_slices;
    double dtheta = pi / sphere_stacks;

    double theta = -pi / 2;
    for (int y = 0; y < sphere_stacks; y++, theta += dtheta) {
        double phi = 0;
        glBegin(GL_QUAD_STRIP);
        for (int x = 0; x <= sphere_slices; x++, phi += dphi) {
            sphere_point(sphere_r, phi, theta);
            sphere_point(sphere_r, phi, theta + dtheta);
        }
        glEnd();
    }
}

void cone_point(double cone_r, double phi, double cone_h) {
    glNormal3d(
        cos(phi),
        sin(phi),
        1
    );
    glVertex3d(
        cone_r * cos(phi),
        cone_r * sin(phi),
        cone_h
    );
}

void draw_cone(double cone_r, int cone_slices, double cone_h) {
    double dphi = 2 * pi / cone_slices;
    double phi = 0;
    glBegin(GL_TRIANGLES);
    for (int x = 0; x < cone_slices; x++, phi += dphi) {
        cone_point(cone_r, phi, 0);
        cone_point(cone_r, phi + dphi, 0);
        glNormal3d(
            0,
            0,
            1
        );
        glVertex3d(
            0,
            0,
            cone_h
        );
    }
    glEnd();
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

void draw_axes() {
    glDisable(GL_LIGHTING);
    glLineWidth(2.0);

    glBegin(GL_LINES);
    glColor3d(1, 0, 0);
    glVertex3d(0, 0, 0);
    glVertex3d(5, 0, 0);

    glColor3d(0, 1, 0);
    glVertex3d(0, 0, 0);
    glVertex3d(0, 5, 0);

    glColor3d(0, 0, 1);
    glVertex3d(0, 0, 0);
    glVertex3d(0, 0, 5);
    glEnd();
    glEnable(GL_LIGHTING);
}

void display() {
    glClearColor(0.85, 0, 0.85, 0);
    //draw_axes();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    glRotated(90, 1, 0, 0);
    glColor3d(1, 1, 0);
    glutSolidTeapot(2);
    glPopMatrix();

    glPushMatrix();
    glColor3d(0, 0.5, 0.5);
    glRotated(cone_angle, 0, 1, 0);
    draw_tor(0.5,10, 60, 30);
    glPopMatrix();

    glPushMatrix();
    //draw_axes();
    glColor3d(0, 0, 1);
    glRotated(cone_angle, 0, 0, 1);
    glTranslated(0, 5, 0);
    draw_cone(cone_r, cone_slices, cone_h);

    glPushMatrix();
    glColor3d(0, 1, 0);
    glRotated(sphere_angle, 0, 0, 1);
    glTranslated(0, 1, 0);
    draw_sphere(sphere_r, sphere_stacks, sphere_slices);
    //                              / 
    glPopMatrix();

    glPopMatrix();

    //glPushMatrix();
    //glColor3d(0, 1, 0);
    //glRotated(0, 0, 0, 0);
    //draw_sphere(sphere_r, sphere_stacks, sphere_slices);
    ////                              / 
    //glPopMatrix();

    //                                 
    glFinish();

    cone_angle += 0.5;
    sphere_angle += 0.8;

    //                                         ,
    //                                                 
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
