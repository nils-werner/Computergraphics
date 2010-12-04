/*
Computergraphics WS2010

Volker Breuer - 21263916
Nils Werner - 21202217
Felix Gundlack - 21309819
*/

#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define PI 3.1415926
#define SPHERE_SEGMENTS 35

void moon();
void earth();
void mars();

float time = 0;
float speed = 0;
float distance, azim_angle, inc_angle, twist = 0;
float last_x, last_y;
bool rotate = 0;
bool circles = 1;
bool orbits = 1;

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
	if(orbits == 0)
		return;
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

void planet(float radius)
{
	if(circles == 1)
	{
		glutSolidSphere(radius, SPHERE_SEGMENTS, SPHERE_SEGMENTS);
	}
	else
	{
		glutSolidCube(radius*2);
	}
}

void sun() 
{
	glColor3f(1.0, 1.0, 0.0);
	planet(0.8);

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
	planet(0.3);

	moon();
		
	glPopMatrix();

	orbit(1.8, 0.0, 0.0);		
}

void moon() 
{
	glPushMatrix();
	//glRotatef(-45.0, 0.0, 0.0, 1.0);
	glRotatef(2*time, 0.0, 0.0, 1.0);
	glTranslatef(0.6, 0.0, 0.0); // temporäre werte
	glColor3f(0.5, 0.5, 0.5);
	planet(0.1);
	glPopMatrix();
	
	orbit(0.6, 0.0, 0.0);
}

void mars() 
{
	glPushMatrix();
	//glRotatef(-30.0, 0.0, 0.0, 1.0);
	glRotatef(0.5*time, 0.0, 0.0, 1.0);
	glTranslatef(3.0, 0.0, 0.0); // temporäre werte
	glColor3f(1.0, 0.0, 0.0);
	planet(0.2);
	glPopMatrix();

	orbit(3.0, 0.0, 0.0);
}

void manipulate_view(float distance, float azimuth, float incidence, float twist)
{
    glTranslatef( 0.0, 0.0, -distance);
    glRotatef( -twist, 0.0, 0.0, 1.0);
    glRotatef( -incidence, 1.0, 0.0, 0.0);
    glRotatef( -azimuth, 0.0, 0.0, 1.0);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glPushMatrix();	
	manipulate_view(distance, azim_angle, inc_angle, twist);

	sun();

	//glFlush();
	glPopMatrix();
	glutSwapBuffers();
}

void idle()
{
	if(rotate == 1)
	{
		time += speed;
		if(fabs(time) > 360*20) // die langsamste geschwindigkeit eines planeten ist 1/20*time
			time = 0;
	}

	glutPostRedisplay();
}


void reshape(int w, int h)
{
        glViewport(0,0,(GLsizei) w, (GLsizei) h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(30.0, (GLfloat) w/ (GLfloat) h, 0.1, 100.0);
        glMatrixMode(GL_MODELVIEW);
}


void keyboard(unsigned char key, int x, int y)
{
	switch(key) {
		case 'c':
		case 'C':
			circles = !circles;
			break;
		case 'r':
		case 'R':
			rotate = !rotate;
			break;
		case 'o':
		case 'O':
			orbits = !orbits;
			break;
		case '+':
			speed += 0.1;
			break;
		case '-':
			speed -= 0.1;
			break;
		case 'w':
		case 'W':
			distance -= 0.1;
		case 'q':
		case 'Q':
			twist += 0.1;
			break;
		case 'e':
		case 'E':
			twist -= 0.1;
			break;
		case 27:
			exit(0);
	}
}

void mouseclick(int button, int state, int x, int y)
{
}

void mousemove(int x, int y)
{
	azim_angle += (x - last_x);
	inc_angle -= (y-last_y);
	
	last_x = x;
	last_y = y;
}

int main(int argc, char **argv)
{
	if(argc < 2)
	{
		printf("Usage: %s <speed>\n", argv[0]);
		printf("Hotkeys available:\n");
		printf("  r       start rotation\n");
		printf("  c       toggle circles/squares\n");
		printf("  o       hide orbits\n");
		printf("  +/-     increase/decrease speed\n");
		printf("  q       quit\n");
		return 1;
	}
	glutInit(&argc, argv);
	speed = atof(argv[1]);
	glutInitWindowSize(600, 600);
	glutCreateWindow("A4.3 - Planeten");
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
        glutMotionFunc(mousemove);
        glutMouseFunc(mouseclick);

	init_openGL();

	glutMainLoop();
}

