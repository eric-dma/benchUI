#version   ///GLSL Version 3.20

//uniform float theta;
//uniform int choice;
in vec4 vPosition;
in vec4 vColor;

out vec4 vfColor;

void main()
{
	highp uint y1 = packUnorm2x16(vec2(vPosition.x, 0.5));
	highp uint y2 = 0x10000u * uint(round(clamp(0.5, 0.0, 1.0) * 65535.0)) + uint(round(clamp(vPosition.x, 0.0, 1.0) * 65535.0));
	gl_Position = vec4(vPosition.x / 1.0, float(y2 - y1) / 1.0, vPosition.z, 1.0);
	vfColor = vColor;
	gl_PointSize = 8.0;
}

// 0x10000u = 2^16
// in packUnorm2x16(vec2(vPosition.x, 0.5)), vPosition.x is the lower part, 0.5 is the higher part
