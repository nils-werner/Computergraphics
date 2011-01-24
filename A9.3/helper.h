#include <math.h>

//***************************************************
// Vector class representing a 3D vector
//***************************************************

class vec3 {
 
  public:
    float x,y,z;
    
	vec3(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	vec3() {
		x = 0.0;
		y = 0.0;
		z = 0.0;
	}
	
	inline float length() {
		return sqrt(x*x+y*y+z*z);
	}	

	inline void normalize() {

		float l=this->length();

		x /= l;
		y /= l;
		z /= l;
	}
 
 	inline vec3 operator+(vec3 B) {
	  
		vec3 C;
		C.x = x+B.x;
		C.y = y+B.y;
		C.z = z+B.z;
		return C;
	}	
	
	inline vec3 operator-(vec3 B) {

	      vec3 C;
		C.x = x-B.x;
		C.y = y-B.y;
		C.z = z-B.z;
		return C;
	}	
	inline vec3 operator*(float a) {
		vec3 C;
		C.x = a*x;
		C.y = a*y;
		C.z = a*z;
		return C;
	}
	
	inline vec3 cross(vec3 B) {
	  vec3 C;
	  C.x = y*B.z - z*B.y;
	  C.y = z*B.x - x*B.z;
	  C.z = x*B.y - y*B.x;
	  
	  return C;
	}
  
};

//*******************************************************************
// A very small matrix class which represents a 3x3 rotation matrix
//*******************************************************************

class rotMat3x3 {
 
  public:
  
  float entries[3][3];
  
    //! default contructor creates identity matrix:
    rotMat3x3() {
      
      entries[0][0] = 1.0;
      entries[0][1] = 0.0;
      entries[0][2] = 0.0;
      entries[1][0] = 0.0;
      entries[1][1] = 1.0;
      entries[1][2] = 0.0;
      entries[2][0] = 0.0;
      entries[2][1] = 0.0;
      entries[2][2] = 1.0;    
    }
  
  //! contructor which creates a rotation matrix from an angle and an axis (rodrigues formula):
    rotMat3x3(vec3 axis, float angle) {
      
      float c = cos(angle*M_PI/180.0);
      float s = sin(angle*M_PI/180.0);
      
      entries[0][0] = c+ axis.x*axis.x*(1-c);
      entries[0][1] = axis.x*axis.y*(1-c)-axis.z*s;
      entries[0][2] = axis.y*s+axis.x*axis.z*(1-c);
      entries[1][0] = axis.z*s+axis.x*axis.y*(1-c);
      entries[1][1] = c+ axis.y*axis.y*(1-c);
      entries[1][2] = -axis.x*s+axis.y*axis.z*(1-c);
      entries[2][0] = -axis.y*s+axis.x*axis.z*(1-c);
      entries[2][1] = axis.x*s+axis.y*axis.z*(1-c);
      entries[2][2] = c+ axis.z*axis.z*(1-c); 
      
    }
  
};

//**********************************
// Inline-functions for general use
//**********************************

//! Matrix-Vector Multiplication:
inline vec3 operator*(rotMat3x3 A, vec3 b) {
 
  vec3 res;
  res.x = b.x*A.entries[0][0] + b.y*A.entries[0][1] + b.z*A.entries[0][2];
  res.y = b.x*A.entries[1][0] + b.y*A.entries[1][1] + b.z*A.entries[1][2];
  res.z = b.x*A.entries[2][0] + b.y*A.entries[2][1] + b.z*A.entries[2][2];
  
  return res;
}

//! The multiplication operator that allows the scalar value to preceed the vector.
inline vec3 operator*(float a, vec3 b)
{ return b*a; }
