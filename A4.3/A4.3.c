/*
Computergraphics WS2010

Volker Breuer - 21263916
Nils Werner - 21202217
Felix Gundlack - 21309819
*/

#include <GL/glut.h>
#include <math.h>

#define SPHERE_SEGMENTS 25

void init_openGL() 
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 5.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);
}

void orbit(int radius, float center_x, float center_y) {
	glColor3f(1.0, 1.0, 1.0);
}

void sun() {
	glColor3f(1.0, 1.0, 0.0);
	glutSolidSphere(1.3, SPHERE_SEGMENTS, SPHERE_SEGMENTS);
}

void earth() {
	glColor3f(0.0, 0.0, 1.0);
	glutSolidSphere(1.3, SPHERE_SEGMENTS, SPHERE_SEGMENTS);

	orbit(0.6, 0.0, 0.0);
}

void moon() {
	glColor3f(0.5, 0.5, 0.5);
	glutSolidSphere(1.3, SPHERE_SEGMENTS, SPHERE_SEGMENTS);
}

void mars() {
	glColor3f(1.0, 0.0, 0.0);
	glutSolidSphere(1.3, SPHERE_SEGMENTS, SPHERE_SEGMENTS);

	orbit(0.3, 0.0, 0.0);
}

void display()
{
	glColor3f(0.5, 0.5, 0.5);
	glBegin(GL_LINE_STRIP);

	glVertex3f(0.0, 0.0, 0.0);

	int i;
	for(i = 1; i < 20; i++) {

		float degree = 360.0 / i;

		float x = cos(degree);
		float y = sin(degree);

		glVertex3f(x, y, 0.0);
	}

	glEnd();
	glFlush();


	sun();
	earth();
	moon();
	mars();

	glFlush();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(512, 512);
	glutCreateWindow("A4.3 - Planeten");
	glutDisplayFunc(display);

	init_openGL();

	glutMainLoop();
}

