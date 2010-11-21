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
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 50.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(2.0, 2.0, 2.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);
}

void orbit(int radius, float center_x, float center_y) 
{
	float pi = 3.1415926;
	int STEPSIZE = 30;
	int phi_step = (float) (2 * pi / STEPSIZE);

	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINE_LOOP);

	glVertex3f(0.0, 0.0, 0.0);

	int i;
	for(i = 1; i < 20; i++) {

		float x = cos(phi_step*i);
		float y = sin(phi_step*i);

		glVertex3f(x, y, 0.0);
	}

	glEnd();
	glFlush();
}

void sun() 
{
	glColor3f(1.0, 1.0, 0.0);
	glutSolidSphere(0.8, SPHERE_SEGMENTS, SPHERE_SEGMENTS);
}

void earth() 
{
	glColor3f(0.0, 0.0, 1.0);
	glutSolidSphere(0.3, SPHERE_SEGMENTS, SPHERE_SEGMENTS);

	orbit(1.8, 0.0, 0.0);
}

void moon() 
{
	glColor3f(0.5, 0.5, 0.5);
	glutSolidSphere(0.1, SPHERE_SEGMENTS, SPHERE_SEGMENTS);
}

void mars() 
{
	glColor3f(1.0, 0.0, 0.0);
	glutSolidSphere(0.2, SPHERE_SEGMENTS, SPHERE_SEGMENTS);

	orbit(3.0, 0.0, 0.0);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);



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

