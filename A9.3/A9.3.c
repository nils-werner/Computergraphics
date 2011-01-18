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
#include "offLoader.h"

#define PI 3.1415926
#define SPHERE_SEGMENTS 35

void moon();
void earth();
void mars();

float animtime = 0;
float speed = 0;
vec3 eye, at, up;
float last_x, last_y;
float eye_speed=0.0;
bool rotate = 0;
bool circles = 1;
bool orbits = 1;
GLfloat zeroes[] = { 0.0, 0.0, 0.0, 1.0 };

OffObject* shuttle;

void init_openGL() 
{
	eye.z = 20.0;
	at.z = 19.0;
	up.x = 1.0,
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 500.0);
//	glOrtho(-5.0, 5.0, -5.0, 5.0, 0.01, 50.0);	

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
//	gluLookAt(eye.x, eye.y, eye.z, at.x, at.y, at.z, 0.0, 1.0, 0.0);
	
	/*deactivate default ambient lightning*/
	GLfloat ambient[] = {0.0f, 0.0f, 0.0f, 0.0};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	
	glColorMaterial (GL_FRONT, GL_DIFFUSE);
	
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

}

void orbit(float radius, float center_x, float center_y) 
{

	if(orbits == 0)
		return;
		
	GLfloat white[] = {.3, .3, .3, 1.0};
	if(glIsEnabled(GL_LIGHT1) || glIsEnabled(GL_LIGHT0)) // übergangslösung um Orbite ohne Licht zu verstecken
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, white);

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
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, zeroes); // disable emission for everybody else
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

void alpha_proxima()
{
	GLfloat light_position[] = {0.0, 0.0, 500.0, 1.0}; // irgendwie muss die Lichtquelle noch verschoben werden und sich nicht mit der Kamera bewegen
	GLfloat light_ambient[] = {0.0, 0.0, 0.0, 0.0};
	GLfloat light_diffuse[] = {0.4, 0.4, 0.4, 1.0};
	GLfloat light_spot_direction[] = {0.0, 0.0, -1.0};
	GLfloat light_spot_cutoff = 45.0;
	
	glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);	//"mittelstarke, diffuse Komponente"
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_spot_direction);	//"unidirektionales Licht"
	glLightfv(GL_LIGHT0, GL_SPOT_CUTOFF, &light_spot_cutoff);	

	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.0); // temporäre werte
	glPopMatrix();

}

void sun() 
{

	/*setup lightsource 1 at the center of the world*/
	GLfloat light_position[] = {0.0, 0.0, 0.0, 1.0};
	GLfloat light_ambient[] = {0.0, 0.0, 0.0, 0.0};			//"[...]keine ambient Komponente[...]"
	GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};			//"[...]kräftige Diffuse- und"
	GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};		//"Specular Komponenten besitzen."
	
	glLightfv(GL_LIGHT1, GL_AMBIENT,  light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE,  light_diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	
	/*setup material specification of the sun*/
	GLfloat yellow[] = {1.0, 1.0, 0.0, 1.0};
	if(glIsEnabled(GL_LIGHT1)) {
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, yellow);
	}

	/*setup sphere representing the sun*/
	glColor3f(1.0, 1.0, 0.0);
	planet(0.8);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, zeroes); // disable emission for everybody else

	earth();
	mars();
}

void earth() 
{
	glPushMatrix();
	//glRotatef(45.0, 0.0, 0.0, 1.0);
	glRotatef(animtime, 0.0, 0.0, 1.0);
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
	glRotatef(2*animtime, 0.0, 0.0, 1.0);
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
	glRotatef(0.5*animtime, 0.0, 0.0, 1.0);
	glTranslatef(3.0, 0.0, 0.0); // temporäre werte
	glColor3f(1.0, 0.0, 0.0);
	planet(0.2);
	glPopMatrix();

	orbit(3.0, 0.0, 0.0);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glPushMatrix();	
	gluLookAt(eye.x, eye.y, eye.z, at.x, at.y, at.z, up.x, up.y, up.z);

	sun();
	alpha_proxima();

	//glFlush();
	glPopMatrix();
	glutSwapBuffers();
}

void idle()
{
	vec3 tmp_eye = eye;
	if(rotate == 1)
	{
		animtime += speed;
		if(fabs(animtime) > 360*20) // die langsamste geschwindigkeit eines planeten ist 1/20*animtime
			animtime = 0;
	}
	
	
	eye = tmp_eye + ((at-tmp_eye)*eye_speed);
	at = at + ((at-tmp_eye)*eye_speed);

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
	vec3 look = (at-eye) * 0.1;
	vec3 right = (at-eye).cross(up) * 0.05;
	
	switch(key) {
		case '1':
			if(glIsEnabled(GL_LIGHT1))
				glDisable(GL_LIGHT1);
			else
				glEnable(GL_LIGHT1);
			break;
		case '2':
			if(glIsEnabled(GL_LIGHT0))
				glDisable(GL_LIGHT0);
			else
				glEnable(GL_LIGHT0);
			break;
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
			speed += 0.01;
			break;
		case '-':
			speed -= 0.01;
			break;
		case 'w':
		case 'W':
			eye = eye + look;
			at = at + look;
			//eye_speed += 0.01; // schubregelung
			break;
		case 's':
		case 'S':
			eye = eye - look;
			at = at - look;
			//eye_speed -= 0.01; // schubregelung
			break;
		case 'a':
		case 'A':
			eye = eye - right;
			at = at - right;
			break;
		case 'd':
		case 'D':
			eye = eye + right;
			at = at + right;
			break;
		case 'q':
		case 'Q':
			up = rotMat3x3(at-eye, -3) * up;
			break;
		case 'e':
		case 'E':
			up = rotMat3x3(at-eye, 3) * up;
			break;
		case 27:
			exit(0);
	}
}

void mouseclick(int button, int state, int x, int y)
{
	last_x = x;
	last_y = y;
}

void mousemove(int x, int y)
{
	/*
	Kamera wird für x-Bewegungen um Up-Vektor und y-Bewegungen um Rechts-Vektor rotiert
	Rechts-Vektor wird aus Lookat- und Up-Vektoren erzeugt.
	Up-Vektor wird bei y-Bewegungen auch rotiert, um anschließend wieder als Rotationsachse dienen zu koennen
	
	x-Achse ist mit absicht umgekehrt, ist beim Flugzeug-Steuerknüppel genauso!
	*/	
	
	vec3 right = (at-eye).cross(up);  // Vektor der Kamera, der nach rechts zeigt
	
	at = eye + rotMat3x3(up, (last_x - x)/15) * (at-eye); // X-Achse, rotiert um up
	at = eye + rotMat3x3(right, (y - last_y)/15) * (at-eye); // y-Achse, rotiert um right
	
	up = rotMat3x3(right, (y - last_y)/15) * up; // Up-Vektor wird als Rotationsachse verwendet, muss also auch rotiert werden
	last_x = x;
	last_y = y;
}

int main(int argc, char **argv)
{
	if(argc < 2)
	{
		printf("Usage: %s <speed>\n", argv[0]);
		printf("\nHotkeys available:");
		
		printf("\n  Solar system controls\n");
		printf("  r       start/stop rotation\n");
		printf("  c       toggle circles/squares\n");
		printf("  o       show/hide orbit-polygons\n");
		printf("  +/-     increase/decrease orbital speed\n");
		
		printf("\n  Camera Movements\n");
		printf("  w/s     move camera forward/backwards\n");
		printf("  a/d     strafe camera left/right\n");
		printf("  l_mouse use mouse for camera orientation\n");
		printf("  q/e     tilt camera left/right\n");
		
		printf("\n  Lightning\n");
		printf("  1       light switch for the sun\n");
		printf("  2       light switch for neighboring star\n");

		printf("\n  Other\n");
		printf("  esq     quit\n");
		return 1;
	}

	/*parse rotation speed*/
	speed = atof(argv[1]);

	/*initialize graphics library*/
	glutInit(&argc, argv);
	glutInitWindowSize(600, 600);
	glutCreateWindow("A7.3 - Planeten");
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);	//handles window resizing

	/*control settings*/
	glutKeyboardFunc(keyboard);
    glutMotionFunc(mousemove);
    glutMouseFunc(mouseclick);

	init_openGL();

	glutMainLoop();
}
