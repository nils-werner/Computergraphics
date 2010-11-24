/*
Computergraphics WS2010

Volker Breuer - 21263916
Nils Werner - 21202217
Felix Gundlack - 21309819
*/

#include <GL/glut.h>
#include <math.h>

#define PI 3.1415926
#define SPHERE_SEGMENTS 35

void moon();
void earth();
void mars();

float time = 0;
bool rotate = 1;

void init_openGL() 
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
//	glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 50.0);
	glOrtho(-5.0, 5.0, -5.0, 5.0, 0.01, 50.0);	

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 20.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);
}

void orbit(float radius, float center_x, float center_y) 
{
	float phi_step = (float) (2 * PI / SPHERE_SEGMENTS);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(center_x, center_y, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	
	glBegin(GL_LINE_LOOP);
	{
		glVertex3f(radius, 0.0, 0.0);

		int i;
		for(i = 1; i <= SPHERE_SEGMENTS; i++) {

			float x = cos(phi_step*i) * radius;
			float y = sin(phi_step*i) * radius;

			glVertex3f(x, y, 0.0);
		}

		glVertex3f(radius, 0.0, 0.0);
	}
	glEnd();
	glPopMatrix();
}

void sun() 
{
	glColor3f(1.0, 1.0, 0.0);
	glutSolidSphere(0.8, SPHERE_SEGMENTS, SPHERE_SEGMENTS);

	earth();
	mars();
}

void earth() 
{
	glPushMatrix();
	//glRotatef(45.0, 0.0, 0.0, 1.0);
	glRotatef(time, 0.0, 0.0, 1.0);
	glTranslatef(1.8, 0.0, 0.0); // temporäre werte
	glColor3f(0.0, 0.0, 1.0);
	glutSolidSphere(0.3, SPHERE_SEGMENTS, SPHERE_SEGMENTS);

	moon();
		
	glPopMatrix();

	orbit(1.8, 0.0, 0.0);		
}

void moon() 
{
	glPushMatrix();
	//glRotatef(-45.0, 0.0, 0.0, 1.0);
	glRotatef(time, 0.0, 0.0, 1.0);
	glTranslatef(0.6, 0.0, 0.0); // temporäre werte
	glColor3f(0.5, 0.5, 0.5);
	glutSolidSphere(0.1, SPHERE_SEGMENTS, SPHERE_SEGMENTS);
	glPopMatrix();
	
	orbit(0.6, 0.0, 0.0);
}

void mars() 
{
	glPushMatrix();
	//glRotatef(-30.0, 0.0, 0.0, 1.0);
	glRotatef(time, 0.0, 0.0, 1.0);
	glTranslatef(3.0, 0.0, 0.0); // temporäre werte
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

	glFlush();
}

void idle()
{
	if(rotate == 1)
	{
		time += 0.1;
		if(time > 360)
			time = 0;
	}
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(600, 600);
	glutCreateWindow("A4.3 - Planeten");
	glutDisplayFunc(display);
	glutIdleFunc(idle);

	init_openGL();

	glutMainLoop();
}

