#version 300 es  
in vec4 a_v4Position;
in vec4 a_v4FillColor;

out vec4 v_v4FillColor;

void main()
{
    v_v4FillColor = a_v4FillColor;
    gl_Position = a_v4Position;
}
