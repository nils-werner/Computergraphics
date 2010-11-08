#include "shape2D/Line2D.h"
#include "ui/Ui.h"
#include <string.h>
#include <math.h>

/*
  implement in this function your normal version of the
  bresenham algorithm.
  For simplicity you only need to implement one octant,
  where x2>x1 and y2>y1 as well as (x2-x1)>(y2-y1).
  Please note, that the origin of the image is in the top
  left corner and that the y-axis is running downwards.
 */
void Line2D::bresenham(Image& image) const
{

	//start position
	int x1 = start.x, y1 = start.y;
	int x2 = end.x, y2 = end.y;

	int x = x1;
	int y = y1;

	int delta_x = x2 - x1;
	int delta_y = y2 - y1;

	int D = delta_x - 2*delta_y;
	int D_E = -2*delta_y;
	int D_NE = 2* (delta_x - delta_y);

	while(x <= x2) {
		image.setColor(x, y, Color(0,1,0));
		x += 1;

		if(D < 0) {
			y = y+1;
			D = D + D_NE;
		} else {
			D = D + D_E;
		}
	}	
}


/*
  After finishing the normal bresenham, copy your code
  into this function and extend it to antialias the lines
 */
void Line2D::antialiasedBresenham(Image& image) const
{
	//start position
	int x1 = start.x, y1 = start.y;
	int x2 = end.x, y2 = end.y;

	int x = x1;
	int y = y1;

	int delta_x = x2 - x1;
	int delta_y = y2 - y1;

	int D = delta_x - 2*delta_y;
	int D_E = -2*delta_y;
	int D_NE = 2* (delta_x - delta_y);

	while(x <= x2) {
		image.setColor(x, y, Color(0,1,0));
		x += 1;

		float a = D / (2*delta_x);

		if(D < 0) {
			y = y+1;
			D = D + D_NE;

			//antialiasing
			float intensity_1 = 1 - fabs(a-0.5);
			float intensity_2 = fabs(a-0.5);			
	
			if(a < 0.5) {

				image.setColor(x+1, y+1, intensity_1 * Color(0,1,0));
				image.setColor(x+1, y, intensity_2 * Color(0,1,0));
			} else {

				image.setColor(x+1, y+1, intensity_1 * Color(0,1,0));
				image.setColor(x+1, y+2, intensity_2 * Color(0,1,0));
			}

		} else {
			D = D + D_E;

			//antialiasing
			float intensity_1 = 1 - fabs(a+0.5);
			float intensity_2 = fabs(a+0.5);			
	
			if(a > -0.5) {

				image.setColor(x+1, y, intensity_1 * Color(0,1,0));
				image.setColor(x+1, y+1, intensity_2 * Color(0,1,0));
			} else {

				image.setColor(x+1, y, intensity_1 * Color(0,1,0));
				image.setColor(x+1, y-1, intensity_2 * Color(0,1,0));
			}
		}
	}
}

// Don't edit below this line
//=================================================================

int main(int argc, char* argv[])
{
  if(argc!=3) {
    cout << "usage:" << endl

	 << argv[0] << " [-antialias;-normal] [-ui;filename]" << endl
	 << "   the first option decides, if the antialiased"
	 << " version should be used" << endl

	 << "   the second option queries, if a window should"
	 << " be opened and display the image, or if it should"
	 << " be written do a file" << endl;
    exit(EXIT_FAILURE);
  }
  Image image(480,480, Color(0,0,0));
  bool doAntialias = (strcmp(argv[1],"-antialias")==0);
  bool showWindow = (strcmp(argv[2], "-ui")==0);
  // draw some lines
  for(int i=0; i<10; i++) {
    Line2D l(Vector2D(10,5*i+5), Vector2D(450, 45*i+5),
	   Color(1,1,1), doAntialias);
    l.draw(image);
  }
  if(showWindow) {
    ImageViewerWindow win("Bresenham", image);
    win.create();
    glutMainLoop();
  } else {
    image.write(argv[2]);
  }
  return EXIT_SUCCESS;
}
