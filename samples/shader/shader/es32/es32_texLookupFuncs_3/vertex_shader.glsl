#version   ///GLSL Version 3.20

in vec4 vPosition;
in vec4 vColor;
uniform highp sampler2D texUnit0;
uniform highp isampler2D texUnit1;
uniform highp usampler2D texUnit2;
uniform highp sampler3D texUnit3;
uniform highp isampler3D texUnit4;
uniform highp usampler3D texUnit5;
uniform highp sampler2DArray texUnit6;
uniform highp isampler2DArray texUnit7;
uniform highp usampler2DArray texUnit8;
uniform highp samplerCube texUnit9;
uniform highp isamplerCube texUnit10;
uniform highp usamplerCube texUnit11;
//uniform highp samplerCubeArray texUnit12;

out vec4 vfColor;


void main()
{

	bool results[32] = bool[32](
					false,false,false,false,false,false,false,false,
					false,false,false,false,false,false,false,false,
					false,false,false,false,false,false,false,false,
					false,false,false,false,false,false,false,false
				);

	vec4 search0 = textureProj(texUnit0, 2.0 * vec3(0.625, 0.375, 1.0));
	results[0] = (search0.y == 6.10);

	ivec4 search1 = textureProj(texUnit1, 2.0 * vec3(0.625, 0.375, 1.0));
	results[1] = (search1.y == 610);

	uvec4 search2 = textureProj(texUnit2, 2.0 * vec3(0.625, 0.375, 1.0));
	results[2] = (search2.y == 610u);

	vec4 search3 = textureProj(texUnit0, 2.0 * vec4(0.625, 0.375, 1.0, 1.0));
	results[3] = (search3.y == 6.10);

	ivec4 search4 = textureProj(texUnit1, 2.0 * vec4(0.625, 0.375, 1.0, 1.0));
	results[4] = (search4.y == 610);

	uvec4 search5 = textureProj(texUnit2, 2.0 * vec4(0.625, 0.375, 1.0, 1.0));
	results[5] = (search5.y == 610u);

	vec4 search6 = textureProj(texUnit3, 2.0 * vec4(0.625, 0.375, 0.375, 1.0));
	results[6] = (search6.y == 22.10);

	ivec4 search7 = textureProj(texUnit4, 2.0 * vec4(0.625, 0.375, 0.375, 1.0));
	results[7] = (search7.y == 2210);

	uvec4 search8 = textureProj(texUnit5, 2.0 * vec4(0.625, 0.375, 0.375, 1.0));
	results[8] = (search8.y == 2210u);

	
	ivec4 search9 = textureLod(texUnit1, vec2(0.75, 0.75), 1.0);
	results[9] = (search9.y == 311);

	uvec4 search10 = textureLod(texUnit2, vec2(0.75, 0.75), 1.0);
	results[10] = (search10.y == 311u);

	ivec4 search11 = textureLod(texUnit4, vec3(0.75, 0.75, 0.25), 1.0);
	results[11] = (search11.y == 311);

	uvec4 search12 = textureLod(texUnit5, vec3(0.75, 0.75, 0.25), 1.0);
	results[12] = (search12.y == 311u);


	ivec4 search13 = textureLod(texUnit7, vec3(0.75, 0.75, 1.0), 1.0);
	results[13] = (search13.y == 711);

	uvec4 search14 = textureLod(texUnit8, vec3(0.75, 0.75, 1.0), 1.0);
	results[14] = (search14.y == 711u);


	vec4 search15 = textureOffset(texUnit0, vec2(0.625, 0.375), ivec2(1, 0));
	results[15] = (search15.y == (6.10 + 1.0));

	ivec4 search16 = textureOffset(texUnit1, vec2(0.625, 0.375), ivec2(1, 0));
	results[16] = (search16.y == (610 + 100));

	uvec4 search17 = textureOffset(texUnit2, vec2(0.625, 0.375), ivec2(1, 0));
	results[17] = (search17.y == (610u + 100u));


	vec4 search18 = textureOffset(texUnit3, vec3(0.625, 0.375, 0.375), ivec3(1, 0, 0));
	results[18] = (search18.y == (22.10 + 1.0));

	ivec4 search19 = textureOffset(texUnit4, vec3(0.625, 0.375, 0.375), ivec3(1, 0, 0));
	results[19] = (search19.y == (2210 + 100));

	uvec4 search20 = textureOffset(texUnit5, vec3(0.625, 0.375, 0.375), ivec3(1, 0, 0));
	results[20] = (search20.y == (2210u + 100u));


	vec4 search21 = textureOffset(texUnit6, vec3(0.625, 0.375, 1.0), ivec2(1, 0));
	results[21] = (search21.y == (22.10 + 1.0));

	ivec4 search22 = textureOffset(texUnit7, vec3(0.625, 0.375, 1.0), ivec2(1, 0));
	results[22] = (search22.y == (2210 + 100));

	uvec4 search23 = textureOffset(texUnit8, vec3(0.625, 0.375, 1.0), ivec2(1, 0));
	results[23] = (search23.y == (2210u + 100u));


	vec4 search24 = texelFetch(texUnit0, ivec2(2, 1), 0);
	results[24] = (search24.y == 6.10);

	ivec4 search25 = texelFetch(texUnit1, ivec2(2, 1), 0);
	results[25] = (search25.y == 610);

	uvec4 search26 = texelFetch(texUnit2, ivec2(2, 1), 0);
	results[26] = (search26.y == 610u);

	vec4 search27 = texelFetch(texUnit3, ivec3(2, 1, 1), 0);
	results[27] = (search27.y == 22.10);

	ivec4 search28 = texelFetch(texUnit4, ivec3(2, 1, 1), 0);
	results[28] = (search28.y == 2210);

	uvec4 search29 = texelFetch(texUnit5, ivec3(2, 1, 1), 0);
	results[29] = (search29.y == 2210u);







	



	











	gl_Position = vec4(vPosition.x / 16.0, float(results[gl_VertexID]) / 2.0, vPosition.z, 1.0);
//	gl_Position = vec4(vPosition.x / 16.0, float(search21.x) / 2.0 , vPosition.z, 1.0);

	if(gl_VertexID < 10)
		vfColor = vColor;
	else if(gl_VertexID < 20)
		vfColor = vec4(0.0, 0.0, 1.0, 1.0);
	else
		vfColor = vec4(0.0, 1.0, 0.0, 1.0);


		
	gl_PointSize = 8.0;
}

// 0x10000u = 2^16
// in packUnorm2x16(vec2(vPosition.x, 0.5)), vPosition.x is the lower part, 0.5 is the higher part
