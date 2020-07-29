#version   ///GLSL Version 3.20

//uniform float theta;
//uniform int choice;
in vec4 vPosition;
in vec4 vColor;

out vec4 vfColor;

void main()
{
	float y1 = vPosition.x;
	float y2 = (unpackSnorm2x16(packSnorm2x16(vec2(vPosition.x, 0.5)))).x;
	gl_Position = vec4(vPosition.x / 1.0, float(y2 - y1) / 1.0, vPosition.z, 1.0);
	vfColor = vColor;
	gl_PointSize = 8.0;
}

// 0x10000u = 2^16
// in packUnorm2x16(vec2(vPosition.x, 0.5)), vPosition.x is the lower part, 0.5 is the higher part
