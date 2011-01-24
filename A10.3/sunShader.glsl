uniform float Time;  // global time variable from the application

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() {
	float Rand = rand(vec2(gl_Vertex.y,gl_Vertex.x));
	float Rad = Time* Rand;
	

	gl_Position = 
		gl_ModelViewProjectionMatrix * 
			(gl_Vertex  + Rad *vec4(gl_Normal, 0)*.1);
}
