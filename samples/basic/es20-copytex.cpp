/* =================================================================
 * Copyright (c) 2019 Botson Corp
 * Botson Confidential and Proprietary
 * =================================================================
 */

/**
 * @file copytex.cpp
 * @author Otto.Liu
 * @par History:
 *    A001: Otto.Liu, 2019-08-26, Add commit
 * @brief basic test for copytex related API
 * @par TestPoints:
 *
 */

#include "Sample.h"

using std::string;
using namespace BotsonSDK;

static const GLubyte indices[] = {
     0,  1,  2,  3
};

static const GLfloat vertices[] = {
    -0.800000f,  0.8000f, 0.0f,
    -0.800000f, -0.8000f, 0.0f,
     0.800000f,  0.8000f, 0.0f,
     0.800000f, -0.8000f, 0.0f,
};

GLfloat clearcolor[5][4] = {
    {1.0f, 0.0f, 0.0f, 1.0f},
    {1.0f, 1.0f, 1.0f, 0.0f},
    {1.0f, 1.0f, 1.0f, 0.0f},
    {0.0f, 1.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 1.0f, 0.0f}
};

static const GLfloat textureCoordinates[] =
{
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
};

GLuint textureID = 0;

GLuint vertexShaderID = 0;
GLuint fragmentShaderID = 0;
GLuint programID = 0;
GLint iLocPosition = -1;
GLint iLocTexCoord = -1;
GLint iLocSampler = -1;

GLenum format[] = {
	GL_ALPHA,
	GL_LUMINANCE,
	GL_LUMINANCE_ALPHA,
	GL_RGB,
	GL_RGBA
};

bool setupGraphics(int w, int h) {
    string vertexShaderPath = ((string)GLSL_PATH) + "basic.vert"; 
    string fragmentShaderPath = ((string)GLSL_PATH) + "basic.frag";

    Shader::processShader(&vertexShaderID, vertexShaderPath.c_str(), GL_VERTEX_SHADER);
    LOGI("vertexShaderID = %d", vertexShaderID);
    Shader::processShader(&fragmentShaderID, fragmentShaderPath.c_str(), GL_FRAGMENT_SHADER);
    LOGI("fragmentShaderID = %d", fragmentShaderID);
    programID = GL_CHECK(glCreateProgram());
    if (!programID) {
        LOGE("Could not create program.");
        return false;
    }

    GL_CHECK(glAttachShader(programID, vertexShaderID));
    GL_CHECK(glAttachShader(programID, fragmentShaderID));
    GL_CHECK(glLinkProgram(programID));
    GL_CHECK(glUseProgram(programID));

    iLocPosition = GL_CHECK(glGetAttribLocation(programID, "a_v4Position"));
    if(iLocPosition == -1) {
        LOGE("Attribute not found: \"a_v4Position\"");
        exit(1);
    }
    GL_CHECK(glEnableVertexAttribArray(iLocPosition));

    iLocTexCoord = GL_CHECK(glGetAttribLocation(programID, "a_v2TexCoord"));
    if(iLocTexCoord == -1) {
        LOGW("Warning: Attribute not found: \"a_v2TexCoord\"");
    } else {
        GL_CHECK(glEnableVertexAttribArray(iLocTexCoord));
    }

    iLocSampler = GL_CHECK(glGetUniformLocation(programID, "u_s2dTexture"));
    if(iLocSampler == -1) {
        LOGW("Warning: Uniform not found: \"u_s2dTexture\"");
    } else {
        GL_CHECK(glUniform1i(iLocSampler, 0));
    }

    return true;
}

void renderFrame(GLenum format, int n) {
    GL_CHECK(glEnable(GL_DEPTH_TEST));
    glViewport(0,0,256,256);
    GL_CHECK(glClearColor(clearcolor[n][0], clearcolor[n][1], clearcolor[n][2], clearcolor[n][3]));
    GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    GL_CHECK(glGenTextures(1, &textureID));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, textureID));
    glCopyTexImage2D(GL_TEXTURE_2D, 0, format, 0, 0, 256, 256, 0);
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));

    glViewport(0,0,WINDOW_W,WINDOW_H);
    GL_CHECK(glClearColor(1.0f, 1.0f, 1.0f, 1.0));
    GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    GL_CHECK(glUseProgram(programID));

    GL_CHECK(glEnableVertexAttribArray(iLocPosition));
    GL_CHECK(glVertexAttribPointer(iLocPosition, 3, GL_FLOAT, GL_FALSE, 0, vertices));
    if(iLocTexCoord != -1) {
        GL_CHECK(glVertexAttribPointer(iLocTexCoord, 2, GL_FLOAT, GL_FALSE, 0, textureCoordinates));
        GL_CHECK(glEnableVertexAttribArray(iLocTexCoord));
    }

    GL_CHECK(glActiveTexture(GL_TEXTURE0));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, textureID));

    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);
}

void Sample::test() {
    for(int i=0; i<ARRAY_NUM(format); i++) {
        setupGraphics(WINDOW_W, WINDOW_H);
        renderFrame(format[i], i);
        string mode_name=get_enum_name(format[i]);
        string bmp_file=filename+"-"+mode_name+".bmp";
        LOGI("bmp_file=%s",bmp_file.c_str());
        save2bmp(bmp_file.c_str());
        Sample::SwapBuffers();
    }
}

int main(int argc, char **argv) {
    Sample sample(__FILE__);

    sample.test();

    return 0;
}