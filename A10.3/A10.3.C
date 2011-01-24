// Hinweise zur Portierung des Shaders in ein bestehendes Planeten-Projekt:
// Alle Codesegmente die als SHADER-RELEVANT markiert sind werden benoetigt.
// Der Name des globale Zeitparameters (in diesem Fall t) bei der Uebergabe 
// an den Shader muss angepasst werden.
// Im Makefile muss die Linker-Option -lGLEW hinzugefuegt werden. 
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <math.h>

#include <GL/glew.h>   /// SHADER-RELEVANT
#include <GL/glut.h>

using namespace std;

// Global Variables %%%%%%%%%%%%%%%%%%%%%%%%%%

float speed;  
float t=0.0;

float orbEarth = 1.8;  // Radius of the Earh-Orbit relative to the sun
float orbMoon = 0.6;   // Radius of the Moon-Orbit relative to the earth
float orbMars = 3.0;   // Radius of the Mars-Orbit relative to the sun

float radSun = 0.8;	// Radius of the sun
float radEarth = 0.3;  // Radius of the Earh
float radMoon = 0.1;   // Radius of the Moon
float radMars = 0.2;   // Radius of the Mars

// Colors:
GLfloat colorSun[] = {0.9,0.9,0.2};
GLfloat colorMars[] = {0.8,0.3,0.2};
GLfloat colorEarth[] = {0.2,0.2,0.7};
GLfloat colorMoon[] = {0.6,0.6,0.6};

GLuint sunShader;  /// SHADER-RELEVANT

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


/// SHADER-RELEVANT -----begin------

// this function creates a shader object by compiling and linking
// the shader code in the file "VSfile".
  void createVertexProgram(const char*VSfile, GLuint *handle) {
    
    GLint compiled;
    char infoLog[4096];
    int infoLogLength;
    
    filebuf *pbuf;
    ifstream filestr;
    int size;
    char *buffer;
    
    filestr.open(VSfile);
    
    // get pointer to associated buffer object
    pbuf=filestr.rdbuf();
    size=pbuf->pubseekoff (0,ios::end,ios::in);
    pbuf->pubseekpos (0,ios::in);
    
    // allocate memory to contain file data
    buffer=new char[size];
    pbuf->sgetn(buffer,size);
    
    const char *VshaderCode = buffer;
    
    filestr.close();
    
    //compile vertex shader: 	
    GLuint Vshader= glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(Vshader,1,&VshaderCode,&size);
    glCompileShader(Vshader);
    glGetShaderiv(Vshader,GL_COMPILE_STATUS, &compiled);
    if ( !compiled) {
      // Print out the info log
      glGetShaderInfoLog(Vshader, sizeof(infoLog), &infoLogLength, infoLog);
      if(infoLogLength > 0)
      {
	printf("CompileShader() infoLog  \n%s\n", infoLog);
      }
    }	
    
    *handle = glCreateProgram();
    glAttachShader(*handle, Vshader);
    
    glDeleteShader(Vshader);
    delete buffer;
    glLinkProgram(sunShader);
  }
 /// SHADER-RELEVANT---- end------ 



  void init_openGL() {
    
    glClearColor(0,0,0,0);
    
    glEnable(GL_DEPTH_TEST);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0,0,5,0,0,0,0,1,0);
    
    createVertexProgram("sunShader.glsl",&sunShader);  /// SHADER-RELEVANT
    
    
    
  }

  void reshape(int w, int h)
  {
    glViewport(0,0,(GLsizei) w, (GLsizei) h);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70.0, (GLfloat) w/ (GLfloat) h, 0.1, 20.0);
    glMatrixMode(GL_MODELVIEW);
  }

  
  void onIdle() {

  t=t+speed;


  glutPostRedisplay();

  }

  void display()
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
    
    
    glPushMatrix();
    
    glRotatef(-25,-3.0,1.0,5.0);  // Tilt of the entire system.
    
    
    // Orbits of the earth and mars drawn as circles:	
    glColor3fv(colorEarth);
    glBegin(GL_LINE_LOOP);	
    for (int i=0; i<360; i++) {
      glVertex3f(orbEarth*cos(i*M_PI/180.0),0,orbEarth*sin(i*M_PI/180.0)); 
    }
    glEnd();
    
    glColor3fv(colorMars);
    glBegin(GL_LINE_LOOP);	
    for (int i=0; i<360; i++) {
      glVertex3f(orbMars*cos(i*M_PI/180.0),0,orbMars*sin(i*M_PI/180.0)); 
    }
    
    glEnd();
    
    // The sun
    glPushMatrix();
    glRotatef(90,1,0,0);  // ... to get the pole of the sphere (with ist ugly artifacts) to the top.
    
    
    /// SHADER-RELEVANT ----begin---- 
    // Activate the sun shader and provide the time-parameter. 
    glUseProgram(sunShader);
    GLint location;
    location = glGetUniformLocation(sunShader, "Time");
    glUniform1f(location, t); // change t to the name of your global time-parameter when you integrate the shader in your project.
    ///SHADER-RELEVANT -----end------

    // Draw the sun:  
    glutSolidSphere(radSun,100,50);
    
    // Deactivate the shader again
    glUseProgram(0);  /// SHADER-RELEVANT
    
    

    
    glPopMatrix();
    
    glPushMatrix();
    
    // The Mars:	  
    glRotatef(-30,0,1,0);
    glTranslatef(0,0,orbMars);
    
    glColor3fv(colorMars);	  
    glutSolidSphere(radMars,40,20);
    
    glPopMatrix();
    
    
    //the Earth:	  
    glRotatef(45,0,1,0); 
    glTranslatef(0,0,orbEarth);
    
    glColor3fv(colorEarth);
    glutSolidSphere(radEarth,50,25);
    
    // the Moon:   
    glRotatef(-30,1,0,0);
    glTranslatef(0,0,orbMoon);
    
    glColor3fv(colorMoon);
    glutSolidSphere(radMoon,30,15);	
    
    
    glPopMatrix();
    
    glutSwapBuffers();
    
    
  }


  void keyboard(unsigned char key, int x, int y) {
    
    switch(key) {
      
      // exit program by pressing ESC: 
      case 27: 		
	glDeleteProgram(sunShader);  /// SHADER-RELEVANT 
	exit(0);
	break;
	
	
    }
    
    glutPostRedisplay();	
  }

  int main(int argc, char **argv)
  {
  if (argc != 2) {
    std::cout << "usage: runme <speed>\n";
    exit(0);
  }

  speed = atof(argv[1]);

  glutInit(&argc, argv);
  glutInitWindowSize(800, 600);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutCreateWindow("Sun Shader");
  GLenum err = glewInit();
  if (GLEW_OK != err)
  {
    fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
  }
  init_openGL();

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutIdleFunc(onIdle);
  glutKeyboardFunc(keyboard);

  glutMainLoop();



  return 0;
  }
