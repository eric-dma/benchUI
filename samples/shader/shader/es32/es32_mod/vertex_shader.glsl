#version   ///GLSL Version 3.20

//uniform float theta;
//uniform int choice;

in vec4 vPosition;
in vec4 vColor;

out vec4 vfColor;

void main()
{
	float y1 = mod(vPosition.x, 3.0);
	vec2 y2 = mod(vec2(1.0, vPosition.x), 3.0);
	vec3 y3 = mod(vec3(1.0, 1.0, vPosition.x), 3.0);
	vec4 y4 = mod(vec4(1.0, 1.0, 1.0, vPosition.x), 3.0);
	float y_average = 0.25 * (y1 + y2.y + y3.z + y4.w);

	gl_Position = vec4(vPosition.x / 20.0, y_average / 3.0, vPosition.z, 1.0); 
	vfColor = vColor;
	gl_PointSize = 8.0;

}
