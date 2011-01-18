#include <iostream>
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
	FILE* file;
	file = fopen(filename.c_str(),"r");
	
	if(!file) {
		cerr << "failed to open: ["<<filename<<"]\n";
	} else {
		char cstring[256];
		int num_v, num_f, num_e;
		vec3 normal;

		while(!feof(file)) {
			fscanf(file, "%s", cstring); //OFF
			fscanf(file, "%d %d %d", &num_v, &num_f, &num_e);
			
			//parse verts
			for(int v=0; v<num_v; v++) {
				vec3 vertex;

				fscanf(file,"%f %f %f", &vertex.x, &vertex.y, &vertex.z);	
				//cout << v << "- x: " << vertex.x << " y: " << vertex.y <<" z: "<< vertex.z << endl;
				vertexList.push_back(vertex);
				normalsList.push_back(vec3(0.0,0.0,0.0)); //space for further editing
			}
			if((int) vertexList.size() == num_v) {
				cout << "vertices loaded" << endl;	
			}

			//parse faces
			for(int f=0; f<num_f; f++) {
				int space;
				vec3 edge1, edge2, local_normal;
				tri face;				

				fscanf(file,"%d %d %d %d",&space, &face.A, &face.B, &face.C);
				//cout << "A: " << face.A << " B: " << face.B <<" C: "<< face.C << endl;
				faceList.push_back(face);
				
				//calc normals for this face
				// step 1: calc the local normal for this face
				edge1 = vertexList[face.B] - vertexList[face.A];
				edge2 = vertexList[face.C] - vertexList[face.A];
				local_normal = edge1.cross(edge2);
				
				// step 2: add normal to the normalbuffer, but add with prev entrys
				normalsList[face.A] = normalsList[face.A] + local_normal; 
				normalsList[face.B] = normalsList[face.B] + local_normal;
				normalsList[face.C] = normalsList[face.C] + local_normal;
			}
			// step 3: normalize all normals after add-operations
			for(int nn=0; nn<num_v; nn++) {
				normalsList[nn].normalize();
			}

			if((int) faceList.size() == num_f) {
				cout << "faces loaded\nnormals calculated" << endl;	
			}
			break;
		}
		fclose(file);
	}
}
