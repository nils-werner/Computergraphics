#include "shape2D/Line2D.h"
#include "ui/Ui.h"
#include <string.h>
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
	const unsigned int dy = end.y - start.y;
	const unsigned int dx = end.x - start.x;
	int x0 = start.x;
	int y0 = start.y;

	int x = x0;
	int y = y0;
	int D = 2*dy-dx;
	int dD_E = 2*dy;
	int dD_NE = 2*(dy-dx);
	//cout << "start---------------"  << endl;
	//cout  << "LINE:" << start << end << endl;
	//cout  << "Image:" << image.sizeX() <<"\t"<< image.sizeY() << endl;
	const Color col_ = Color().stdGray; 
	while(x < end.x && x < image.sizeX() && y < image.sizeY()){
		image.setColor (x, y, col_);
		x++;
		if(D > 0){	//NE
			y++;
			D = D + dD_NE;
		}
		else		//E
			D = D + dD_E;
	}
	//cout << "................finish" << endl;
}


/*
  After finishing the normal bresenham, copy your code
  into this function and extend it to antialias the lines
 */
void Line2D::antialiasedBresenham(Image& image) const
{
	const unsigned int dy = end.y - start.y;
	const unsigned int dx = end.x - start.x;
	int x0 = start.x;
	int y0 = start.y;

	int x = x0;
	int y = y0;
	int D = 2*dy-dx;
	int dD_E = 2*dy;
	int dD_NE = 2*(dy-dx);
	Color col_ = Color().white; 
	while(x < end.x && x < image.sizeX() && y < image.sizeY()){
		float a = D/(2*dx);
		x++;
		if(D < 0){
			D = D + dD_E;
			float faktor = fabs(a + 0.5);
			if(a > -0.5){
				image.setColor(x+1, y, (1-faktor)* col_);
				image.setColor(x+1,y+1,faktor*col_);
			}
			else{
				image.setColor(x+1, y, (1-faktor)*col_);
				image.setColor(x+1,y-1, faktor*col_);	
			}
		}
		else{
			y++;
			D = D + dD_NE;
			float faktor = fabs(a - 0.5);
			if(a < 0.5){
				image.setColor(x+1, y+1, (1-faktor) *col_);
				image.setColor(x+1,y,faktor*col_);
			}
			else{
				image.setColor(x+1, y+1, (1-faktor) *col_);
				image.setColor(x+1,y+2,faktor*col_);	
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
