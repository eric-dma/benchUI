#include "Sample.h"
#include "Shader.h"
#include <string>

#define BUFFER_OFFSET(x)  ((const void*) (x))

GLuint programID, VAOs, Buffers;
GLint max_uniform_components;

GLfloat vertices[]={ -0.9f, -0.9f,    0.9f, -0.9f,     0.0f, 0.9f,
                 1.0, 0.0, 0.0,    0.0, 1.0, 0.0,  0.0, 0.0, 1.0
};

void loadShader(void)
{
    GLint success; 
    GLchar infoLog[512];

    //const GLchar* vertexShaderSource = 
    const GLchar* vertexShaderSource =
        "\n#version 430 core\n"
        "layout(location=0) in vec4 position;\n"
        "layout(location=1) in vec4 vcolor;\n"
        "out vec4 color;\n\n"
        "void main()\n"
        "{\n"
        "    gl_Position = position;\n"
        "    color = vcolor;\n"
        "}\n\0";


    std::string tcShaderSource0 =
        "#version 430 core\n"
        "layout(vertices=3) out;\n"
        "in vec4 color[];\n"
        "out vec4 TCcolor[];\n";


    std::string tcShaderSource_uniform0;
    std::string tcShaderSource_main2;
    std::string tcShaderSource_main3;
    std::string tcShaderSource_main4="mat4 scaling =";
    for(int i=0; i<max_uniform_components/16; i++){
       tcShaderSource_uniform0.append("layout(location="+std::to_string(i)+
        ") uniform mat4 scaling"+std::to_string(i)+";\n");
       std::string temp;
       tcShaderSource_main2.append("scaling"+std::to_string(i));
       if(i<max_uniform_components/16-1){
           temp="*";} else {
           temp=" ;\n\0 ";
       }
        tcShaderSource_main2.append(temp);
    }

        tcShaderSource_main3.append("gl_out[gl_InvocationID].gl_Position = scaling*gl_in[gl_InvocationID].gl_Position;\nTCcolor[gl_InvocationID] = color[gl_InvocationID];\n}");
        tcShaderSource_main4.append(tcShaderSource_main2);


    std::string tcShaderSource_main1 =
        "void main()\n"
        "{\n"
        "gl_TessLevelInner[0] = 3.0;\n"
        "gl_TessLevelOuter[0] = 3.0;\n"
        "gl_TessLevelOuter[1] = 3.0;\n"
        "gl_TessLevelOuter[2] = 3.0;\n";

    std::string tcShaderSource_main;
    std::string tcShaderSource_s;

    tcShaderSource_main.append(tcShaderSource_main1);
    tcShaderSource_main.append(tcShaderSource_main4);
    tcShaderSource_main.append(tcShaderSource_main3);
    tcShaderSource_s.append(tcShaderSource0);
    tcShaderSource_s.append(tcShaderSource_uniform0);
    tcShaderSource_s.append(tcShaderSource_main);
    LOGI("tcShaderSource = %s\n", tcShaderSource_s.c_str());
    const GLchar* tcShaderSource = tcShaderSource_s.c_str();

    const GLchar* teShaderSource =
            "\n#version 430 core\n"
            "layout(triangles) in;\n"
            "in vec4 TCcolor[];\n"
            "out vec4 color;\n"
            "void main()\n"
            "{\n"
            "    float u = gl_TessCoord.x;\n"
            "    float v = gl_TessCoord.y;\n"
            "    float w = gl_TessCoord.z;\n"
            "    gl_Position = (u * gl_in[0].gl_Position+ \n"
            "    v * gl_in[1].gl_Position + w * gl_in[2].gl_Position);\n"
            "    color = (TCcolor[0]*(1.0-v) + \n"
            "    + TCcolor[1]*(1.0-u) + + TCcolor[2]*(1.0-w));\n"
            "}\n\0";

    const GLchar* fragmentShaderSource =
        "\n#version 430 core\n"
        "in vec4 color;\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "    FragColor = color;\n"
        "}\n\0";


    programID = Shader::createProgramFromString(vertexShaderSource, tcShaderSource, teShaderSource, fragmentShaderSource);
    LOGI("PROGRAM = %d\n", programID);

    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if(!success) {
            glGetProgramInfoLog(programID, 512, NULL, infoLog); 
            printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n"); 
    }
}


bool setupGraphics(int w, int h)
{
    GL_CHECK(glGetIntegerv(GL_MAX_TESS_CONTROL_UNIFORM_COMPONENTS, &max_uniform_components));
    LOGI("GL_MAX_TESS_CONTROL_UNIFORM_COMPONENTS = %d\n", max_uniform_components);

    loadShader();
    GL_CHECK(glGenVertexArrays(1, &VAOs));
    GL_CHECK(glBindVertexArray(VAOs));
    GL_CHECK(glGenBuffers(1, &Buffers));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, Buffers));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
    GL_CHECK(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0)));
    GL_CHECK(glEnableVertexAttribArray(0));
    GL_CHECK(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(float)*2*4)));
    GL_CHECK(glEnableVertexAttribArray(1));

    GL_CHECK(glViewport(0, 0, w, h));
    
    return true;
} 

void renderFrame(void) {
    GL_CHECK(glClearColor(1.0f, 1.0f, 1.0f, 1.0f));
    GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));
    GL_CHECK(glUseProgram(programID));
    Matrix scale0    = Matrix::createScaling(1.0f, 1.0f, 1.0f);
    Matrix scale1    = Matrix::createScaling(0.3f, 1.0f, 1.0f);
    for(int i=0; i<max_uniform_components/16-1; i++){
        GL_CHECK(glUniformMatrix4fv(i, 1, GL_FALSE, scale0.getAsArray()));
    }
    GL_CHECK(glUniformMatrix4fv((max_uniform_components/16-1), 1, GL_FALSE, scale1.getAsArray()));
    GL_CHECK(glPatchParameteri(GL_PATCH_VERTICES, 3));
    GL_CHECK(glBindVertexArray(VAOs));
    GL_CHECK(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
    GL_CHECK(glDrawArrays(GL_PATCHES, 0, 3));
}

void Sample::test() {
    string bmp_file = filename + ".bmp";
    LOGI("[]: filename=%s\n", bmp_file.c_str());
    setupGraphics(WINDOW_W, WINDOW_H);
    renderFrame();
    save2bmp(bmp_file.c_str());
    SwapBuffers();
}

int main(int argc, char **argv) {
    Sample sample(__FILE__, GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE, argc,argv);

    sample.test();

    return 0;
}
