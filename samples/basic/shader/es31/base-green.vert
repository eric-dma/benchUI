#version 310 es                                  

precision mediump float;

in vec4 a_v4Position;

void main()
{
    gl_Position = a_v4Position;
}
