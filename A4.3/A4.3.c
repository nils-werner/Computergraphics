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
	gluLookAt(0.0, 0.0, 2.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);
}

void orbit(int radius, float center_x, float center_y) 
{
	float pi = 3.1415926;
	int STEPSIZE = 30;
	float phi_step = (float) (2 * pi / STEPSIZE);

	glPushMatrix();
	glTranslatef(center_x, center_y, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINE_LOOP);

	glVertex3f(radius, 0.0, 0.0);

	int i;
	for(i = 1; i <= 20; i++) {

		float x = cos(phi_step*i);
		float y = sin(phi_step*i);

		glVertex3f(x, y, 0.0);
	}

	glVertex3f(radius, 0.0, 0.0);

	glEnd();
	glPopMatrix();
}

void sun() 
{
	glColor3f(1.0, 1.0, 0.0);
	glutSolidSphere(0.8, SPHERE_SEGMENTS, SPHERE_SEGMENTS);
}

void earth() 
{
	glPushMatrix();
	glTranslatef(1.8, 1.0, 0.0); // temporäre werte
	glColor3f(0.0, 0.0, 1.0);
	glutSolidSphere(0.3, SPHERE_SEGMENTS, SPHERE_SEGMENTS);
	glPopMatrix();

	orbit(1.8, 0.0, 0.0);
}

void moon() 
{
	glPushMatrix();
	glTranslatef(1.8, 0.4, 0.0); // temporäre werte
	glColor3f(0.5, 0.5, 0.5);
	glutSolidSphere(0.1, SPHERE_SEGMENTS, SPHERE_SEGMENTS);
	glPopMatrix();
}

void mars() 
{
	glPushMatrix();
	glTranslatef(1.0, -1.0, 0.0); // temporäre werte
	glColor3f(1.0, 0.0, 0.0);
	glutSolidSphere(0.2, SPHERE_SEGMENTS, SPHERE_SEGMENTS);
	glPopMatrix();

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

