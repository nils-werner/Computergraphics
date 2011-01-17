#include <fstream>
#include <vector>

#include "helper.h"

using namespace std;

// simple structure to hold three integer-values (used for triangle indicies)
struct tri {
  int A;
  int B;
  int C;
};


// Declaration:

class OffObject 
{

public: 
	
  	vector<vec3> vertexList;
	vector<vec3> normalsList;
	vector<tri> faceList;
	
	int noOfFaces;
	int noOfVertices;
	
	OffObject(string filename);

};


//TODO: IMPLEMENTATION:

// the constuctor recieves the filename (an off file) and parses it. The vertices, normals and triangles
// are pushed back into the respective container whereas the normals have to be explicitly computetd for each 
// vertex. 

OffObject::OffObject(string filename) {
 
  
  
}
