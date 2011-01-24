uniform float Time;  // global time variable from the application

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() {
	float Rand = rand(vec2(gl_Vertex.y,gl_Vertex.x+gl_Vertex.z));
	float Rad = Time* Rand;
	

	gl_Position = 
		gl_ModelViewProjectionMatrix * 
			(gl_Vertex  + sin(Rad) *vec4(gl_Normal, 0)*.1);
			
	gl_FrontColor = vec4(1.0, sin(Rad)/0.5+0.5, 0.0, 1.0);
}
