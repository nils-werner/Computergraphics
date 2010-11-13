#include "shape2D/Line2D.h"
#include "base/Matrix3x3.h"
#include "ui/Ui.h"

// The program must be started with one command line argument:
//   the number of lines to draw
// The programm will generate a random set of lines and clip them 
// against the outside of a clip rectangle, which is of half size of the
// window.
// Using the push-button, you can switch between the display of the clipped
// and the original lines.

// Compute the window edge coordinates and return them in the variable wecs.
// Also compute the outcodes and return them as bitflags.
// You can make use of the enum type defined in ClipRect:
// TOP_FLAG, BOTTOM_FLAG, RIGHT_FLAG and LEFT_FLAG
int ClipRect::wec(const Vector2D& v, float wecs[4]) const
{
}


// Clip the line against the outside of the clip rectangle. The resulting line-
// segment(s) should be added to the vector of clipped lines.
void Line2D::clipOutside(const ClipRect& clipRect, 
                         vector<Line2D>& clippedLines) const
{
}

// Don't edit below this line
//=================================================================

//! This class represents the main-window.
class ClipWindow : public MainWindow
{
public:
  //! constructor
  ClipWindow(int numLines);
  //! create the window 
  void create();
    
  //! draw the shapes into a window
  virtual void onDraw();
  //! notify, when the window-size has changed
  virtual void onWindowReshape(ScreenPoint newSize);

private:
  //! Toggle the show clipped state
  void showClipped();

  //! The set of original lines
  vector<Line2D> m_lines;
  //! The set of original lines
  vector<Line2D> m_clippedLines;

  //! Show the original or the clipped lines
  bool m_showClipped;

  //! the rectangle to clip against
  ClipRect m_clipRect;

  // A button
  PushButton m_showClippedButton;
};

ClipWindow::ClipWindow(int numLines)
  : MainWindow("Clip Window"), m_showClipped(false),
    m_clipRect(Vector2D(-50,-50),Vector2D(50,50)),
    m_showClippedButton(this,"Show Clipped",ScreenPoint(10,10), 
                        ScreenPoint(130,50),
                        (WindowNotificationFunction)&ClipWindow::showClipped)
{
  // turn-off depth-buffer for this window
  m_DisplayMode &= ~(GLUT_DEPTH|GLUT_ALPHA);

  // Fill the original lines with random
  for(int i=0; i<numLines; ++i) {
    Vector2D start;
    Vector2D end;
    start.x = (rand()/static_cast<float>(RAND_MAX) - 0.5)*200;
    start.y = (rand()/static_cast<float>(RAND_MAX) - 0.5)*200;
    end.x = (rand()/static_cast<float>(RAND_MAX) - 0.5)*200;
    end.y = (rand()/static_cast<float>(RAND_MAX) - 0.5)*200;
    Color color = Color(rand()/static_cast<float>(RAND_MAX),
                        rand()/static_cast<float>(RAND_MAX),
                        rand()/static_cast<float>(RAND_MAX));

    m_lines.push_back(Line2D(start,end, color, false));
    // fill in the clipped lines
    m_lines.back().clipOutside(m_clipRect, m_clippedLines);
  }
  cerr << "Number of original lines: " << m_lines.size() << endl;
  cerr << "Number of clipped lines:  " << m_clippedLines.size() << endl;
}

void ClipWindow::create()
{
  MainWindow::create();
  m_showClippedButton.create();

  // set the camera to an orthogonal one
  m_Camera.setPerspective(false);
  // initialize some OpenGL-stuff
  glClearColor(0.8,0.8,0.8,0);
}

void ClipWindow::showClipped()
{
  m_showClipped = !m_showClipped;
 
  if(m_showClipped) {
    m_showClippedButton.setTitle("Show Original");
  } else {
    m_showClippedButton.setTitle("Show Clipped");
  }
  setActive();
  glutPostRedisplay();
}

void ClipWindow::onDraw()
{
  glClear(GL_COLOR_BUFFER_BIT);
  vector<Line2D>& displayLines = m_showClipped?m_clippedLines:m_lines;

  // for each line
  for(vector<Line2D>::const_iterator iter = displayLines.begin();
      iter!=displayLines.end(); ++iter) {
    iter->drawGL();
  }
  // draw the clip-screen
  m_clipRect.drawGL();
  glutSwapBuffers();
}

void ClipWindow::onWindowReshape(ScreenPoint newSize)
{
  m_Size = newSize;
  glViewport(0,0, m_Size.x, m_Size.y);
  float aspect = (float)newSize.x/newSize.y;
  if(aspect<1)
    m_Camera.setFrustum(-100,100,100/aspect,-100/aspect,-1000,1000);
  else
    m_Camera.setFrustum(-100*aspect,100*aspect,100,-100,-1000,1000);
  m_Camera.applyProjectionToGL();
}

int main(int argc, char* argv[])
{
  if(argc!=2) {
    cerr << "Usage : " << argv[0] << " number-of-lines" << endl;
    return EXIT_FAILURE;
  }
  int numLines = atoi(argv[1]);
  ClipWindow mainWindow(numLines);
  mainWindow.create();
  glutMainLoop();
  return EXIT_SUCCESS;
}
