/*
Computergraphics WS2010

Volker Breuer - 21263916
Nils Werner - 
Felix Gundlack - 
*/

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

//Compute the window edge coordinates and outcode for a given point (v?).
// Compute the window edge coordinates and return them in the variable wecs. 
// Also compute the outcodes and return them as bitflags. 
//You can make use of the enum type defined in ClipRect:
// TOP_FLAG, BOTTOM_FLAG, RIGHT_FLAG and LEFT_FLAG		||		WEC = Window Edge Coordinates
int ClipRect::wec(const Vector2D& v, float wecs[4]) const
{
	//x,y values describing the clip rectangle
	double x_min = minCorner.x;
	double y_min = minCorner.y;
	double x_max = maxCorner.x;
	double y_max = maxCorner.y;

	//calculating wec for given point
	wecs[0] = v.x - x_min;		//WEC_left
	wecs[1] = x_max - v.x;		//WEC_right
	wecs[3] = y_max - v.y;		//WEC_top			//TOP / Bottom exchanged because of top left coord of screen ist 0,0
	wecs[2] = v.y - y_min;		//WEC_bottom

	//compute outcodes
	int outcodes = 0x0;
	outcodes = ((v.x < x_min) * LEFT_FLAG) | ((v.x > x_max) * RIGHT_FLAG) | ((v.y < y_min) * TOP_FLAG) | ((v.y > y_max) * BOTTOM_FLAG);

	return outcodes;
}

// Clip the line against the outside of the clip rectangle. The resulting line-
// segment(s) should be added to the vector of clipped lines.
void Line2D::clipOutside(const ClipRect& clipRect, 
                         vector<Line2D>& clippedLines) const
{
	//describes the positions in the wec-array
	int edge[] = {clipRect.LEFT_FLAG, clipRect.RIGHT_FLAG, clipRect.TOP_FLAG, clipRect.BOTTOM_FLAG};

	//windows edge coordinates and outcodes
	float wecs_start[4];
	float wecs_end[4];
	int outcode_start;
	int outcode_end;

	//compute wec and outcode of start- and endpoint
	outcode_start = clipRect.wec(start, wecs_start);
	outcode_end = clipRect.wec(end, wecs_end);

	//debug information
	cout << "-------------------" << endl
		<< "outCodes: start = " << outcode_start 
		<< " stop = " << outcode_end << endl
		<< "start = ";
	clipRect.displayOutcode(outcode_start);
	cout << "stop = ";
	clipRect.displayOutcode(outcode_end);
	cout << "-------------------" << endl;	

	cout << "wecs_start: " << wecs_start[0] << endl;
	cout << "wecs_start: " << wecs_start[1] << endl;
	cout << "wecs_start: " << wecs_start[2] << endl;
	cout << "wecs_start: " << wecs_start[3] << endl;
	cout << "wecs_end: " << wecs_end[0] << endl;
	cout << "wecs_end: " << wecs_end[1] << endl;
	cout << "wecs_end: " << wecs_end[2] << endl;
	cout << "wecs_end: " << wecs_end[3] << endl;

	//trivial reject: start and endpoint inside
	if ((outcode_start == 0) && (outcode_end == 0)) {
		//line completely inside rectangle -> won't get drawn
		return;
	}

	//for each edge...
	double alpha_min = 0, alpha_max = 1;
	
	int i;
	for(i = 0; i < 4; i++) {

		//Point 'start':
		if(edge[i] & outcode_start) {

			double alpha = wecs_start[i] / (wecs_start[i] - wecs_end[i]);
			alpha_min = (alpha > alpha_min) ? alpha : alpha_min;
		}

		//Point 'end':
		if(edge[i] & outcode_end) {
			double alpha = wecs_start[i] / (wecs_start[i] - wecs_end[i]);
			alpha_max = (alpha < alpha_max) ? alpha : alpha_max;
		}
	}

	cout << "Alpha_Min: " << alpha_min << "; \tAlpha_Max: " << alpha_max << endl;

	if(alpha_min < alpha_max) {
		Vector2D newStart = start + alpha_min * (end - start);
		Vector2D newEnd = end + alpha_max * (end - start);
		clippedLines.push_back(Line2D(newStart, newEnd, color, false));
	}
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
