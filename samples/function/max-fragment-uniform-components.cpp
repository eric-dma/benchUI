/**
 ** @file max-ver-uniform-vec.cpp
 ** @author Cary.Zhang
 ** @par History:
 **    A001: Cary.Zhang, 2020-05-15, create file
 ** @par TestPoints:
 **   GL_MAX_FRAGMENT_UNIFORM_COMPONENTS
 **/


#include "Sample.h"
#include <vector>
using namespace std;

#define BUFFER_OFFSET(x)  ((const void*) (x))

GLfloat vertices[] = {
	0.0f, 0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f
};
 
GLfloat colors[] = { 
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f
}; 


GLuint programID, VAOs, Buffers;;

const GLuint uniformIndices = 0;
const GLuint uniformIndices1 = 1;

GLint params;
GLint params1;

GLint length;
GLchar uniformName;
GLuint indices;
const char*names = {"u1f"};

GLint my_frag_uniform_components;

void loadShader(void)
{
    GLint success; 
    GLchar infoLog[512];

    std::string SvertexShaderSource =
        "#version 430 core\n"
        "layout(location=0) in vec3 position;\n"
        "layout(location=1) in vec4 acolor;\n"
        "out vec4 vcolor;\n"
        "void main()\n"
        "{\n"
        "    vcolor = acolor;\n"
        "    gl_Position = vec4(position, 1.0);\n"
        "}\n\0";


    LOGI("vertexShaderSource = \n %s\n", SvertexShaderSource.c_str());
    const GLchar* vertexShaderSource = SvertexShaderSource.c_str();

    std::string SfragmentShaderSource =
        "#version 430 core\n"
        "in vec4 vcolor;\n"
        "out vec4 FragColor;\n"
        "uniform float[";
    std::string SfragmentShaderSource1 =
        "] color_n;\n"
        "void main()\n"
        "{\n";
    SfragmentShaderSource.append(std::to_string(my_frag_uniform_components));
    SfragmentShaderSource.append(SfragmentShaderSource1);

    std::string vertex_const = "    FragColor = vcolor +";
    std::string vertex_n1 = ""; 
    std::string vertex_temp1 ;

    for(int i = 1 ; i < my_frag_uniform_components ; i++)
    {
        std::string temp;
        if(i < ( my_frag_uniform_components - 1)) 
        {
            temp = " + ";
        }
        else
        {
            temp = " ;\n\0 ";

        }
        vertex_temp1.append(" color_n[" + std::to_string(i) + "]" + temp);
    }   

    vertex_n1.append(vertex_const + vertex_temp1);
    SfragmentShaderSource.append(vertex_n1 + "\n}\n" );

    LOGI("SfragmentShaderSource = \n %s\n", SfragmentShaderSource.c_str());
    const GLchar* fragmentShaderSource = SfragmentShaderSource.c_str();

    programID = Shader::createProgramFromString(vertexShaderSource, fragmentShaderSource);
    LOGI("PROGRAM = %d\n", programID);

    glGetProgramiv(programID, GL_LINK_STATUS, &success); 
    if (!success) { 
            glGetProgramInfoLog(programID, 512, NULL, infoLog); 
            printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n"); 
    }
}


bool setupGraphics()
{
        GL_CHECK(glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &my_frag_uniform_components));
        LOGI("GL_MAX_FRAGMENT_UNIFORM_COMPONENTS = %d\n", my_frag_uniform_components);

        loadShader();

	GL_CHECK(glGenVertexArrays(1, &VAOs));
	GL_CHECK(glBindVertexArray(VAOs));

	GL_CHECK(glGenBuffers(1, &Buffers));
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, Buffers));

	GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), NULL, GL_STATIC_DRAW));
	GL_CHECK(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices));
	GL_CHECK(glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors));
	GL_CHECK(glEnableVertexAttribArray(0));
	GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0));
	GL_CHECK(glEnableVertexAttribArray(1));
	GL_CHECK(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertices))));

    return true;
} 

void renderFrame(void)
{
	GL_CHECK(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
	GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));
	GL_CHECK(glUseProgram(programID));


        vector<GLfloat> max_vec(my_frag_uniform_components, 0.0001);
        GL_CHECK(glUniform1fv(glGetUniformLocation(programID, "max_vec"), my_frag_uniform_components, /*static_cast<GLfloat*> max_vec));*/ &max_vec[0]));

	GL_CHECK(glBindVertexArray(VAOs));

        GL_CHECK(glViewport(0, 0, WINDOW_W, WINDOW_H)); 
        GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, 3));

	GL_CHECK(glFlush());
}

void Sample::test() {
	string bmp_file = filename + ".bmp";
	LOGI("[]: filename=%s\n", bmp_file.c_str());
	setupGraphics();
        renderFrame();
	save2bmp(bmp_file.c_str());
	SwapBuffers();
}

int main(int argc, char **argv) {
    Sample sample(__FILE__, GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE, argc,argv);

    sample.test();

    return 0;
}
