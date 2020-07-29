#version 430 core

precision mediump int;
precision mediump float;
precision mediump sampler1D;
precision mediump sampler2D;
precision mediump sampler3D;
precision mediump samplerCube;
precision mediump sampler2DArray;
precision mediump samplerBuffer;


out vec4 outputColor;
uniform sampler1D texSampler;

void main() {
  outputColor = texture(texSampler, 1.0);
}