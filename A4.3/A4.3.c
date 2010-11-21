/*
Computergraphics WS2010

Volker Breuer - 21263916
Nils Werner - 21202217
Felix Gundlack - 21309819
*/

#include <GL/glut.h>
#include <math.h>

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

/*
	glColor3f(1.0, 1.0, 0.0);
	glutSolidSphere(1.3, 25, 25);
	glFlush();
*/
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

