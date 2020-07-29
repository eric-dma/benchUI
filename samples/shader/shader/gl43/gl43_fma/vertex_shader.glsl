#version   ///GLSL Version 3.20

//uniform float theta;
//uniform int choice;
in vec4 vPosition;
in vec4 vColor;

out vec4 vfColor;

void main()
{
	float y1 = fma(-1.0, 1.0, vPosition.x);
	vec2 y2 = fma(vec2(1.0, -1.0), vec2(1.0, 1.0), vec2(1.0, vPosition.x));
	vec3 y3 = fma(vec3(1.0, 1.0, -1.0), vec3(1.0, 1.0, 1.0), vec3(1.0, 1.0, vPosition.x));
	vec4 y4 = fma(vec4(1.0, 1.0, 1.0, -1.0), vec4(1.0, 1.0, 1.0, 1.0), vec4(1.0, 1.0, 1.0, vPosition.x));
	float y_average = 0.25 * (y1 + y2.y + y3.z + y4.w);

	gl_Position = vec4(vPosition.x / 10.0, y_average / 10.0, vPosition.z, 1.0); 
	vfColor = vColor;
	gl_PointSize = 8.0;
}