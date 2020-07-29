/* =================================================================
 * Copyright (c) 2019 Botson Cor
 * Botson Confidential and Proprietary
 * =================================================================
 */

/**
 * @file sampler-wrap2d.cpp
 * @author Otto.Liu
 * @par History:
 *    A001: Otto.Liu, 2019-09-02, Add commit
 * @brief basic test for sampler-wrap2d related API
 * @par TestPoints:
 *
 */

#include "Sample.h"

using std::string;
using namespace BotsonSDK;

string vertexShaderFilename = "Sampler.vert";
string fragmentShaderFilename = "Sampler.frag";
string textureFileName = "back.jpg";

GLuint programID;
GLuint vertexShaderID = 0;
GLuint fragmentShaderID = 0;
GLint iLocPosition = -1;
GLint iTexPosition = -1;
GLuint vao;
GLuint positionBufferObject;
const int texUnit=0;
GLuint sampler=0;
GLuint samplerUniform;
GLuint texBufferObject;

float vertices[] = {
    -1.0f,  1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,
};

float texcoords[] = {
    -2.0f, 3.0f,
    -2.0f, -2.0f,
    3.0f, 3.0f,
    3.0f, -2.0f,
};

GLenum factors[] = {
	GL_CLAMP_TO_EDGE, 
	GL_MIRRORED_REPEAT,
	GL_REPEAT,
};

bool setupGraphics(int width, int height) {
    string vertexShaderPath = (string)GLSL_PATH + vertexShaderFilename; 
    string fragmentShaderPath = (string)GLSL_PATH + fragmentShaderFilename;
    string textureShaderPath = (string)PICTURE_PATH + textureFileName;

    Shader::processShader(&vertexShaderID, vertexShaderPath.c_str(), GL_VERTEX_SHADER);
    Shader::processShader(&fragmentShaderID, fragmentShaderPath.c_str(), GL_FRAGMENT_SHADER);
    programID = GL_CHECK(glCreateProgram());
    if (programID == 0) {
        LOGE("Could not create program.");
        return false;
    }
    GL_CHECK(glAttachShader(programID, vertexShaderID));
    GL_CHECK(glAttachShader(programID, fragmentShaderID));
    GL_CHECK(glLinkProgram(programID));

    iLocPosition = glGetAttribLocation(programID, "pos");
    iTexPosition = glGetAttribLocation(programID, "tex");

    glUseProgram(programID);

    TexData image_jpg;
    image_jpg.file_name = (char *)textureShaderPath.c_str();
    read_jpeg(&image_jpg);
    glGenTextures(1, &texBufferObject);
    glBindTexture(GL_TEXTURE_2D, texBufferObject);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_jpg.width, image_jpg.height,
                 0, GL_RGB,GL_UNSIGNED_BYTE, image_jpg.data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    free(image_jpg.data);

    glViewport(0, 0, width, height);
    GL_CHECK(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
    GL_CHECK(glClearDepthf(1.0f));

    return true;
}

void renderFrame(GLenum factor) {
    GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    
    glGenSamplers(1,&sampler);
    glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S,factor);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T,factor);
    samplerUniform = glGetUniformLocation  (programID, "texSampler"); 
    glUniform1i(samplerUniform, texUnit);
    GLint param;
    glGetSamplerParameteriv(sampler, GL_TEXTURE_WRAP_S,&param);
    LOGI("glGetSamplerParameteriv(GL_TEXTURE_WRAP_S)=%x",get_enum_name(param));
    glGetSamplerParameteriv(sampler, GL_TEXTURE_WRAP_T,&param);
    LOGI("glGetSamplerParameteriv(GL_TEXTURE_WRAP_T)=%x",get_enum_name(param));

    GL_CHECK(glUseProgram(programID));
    glActiveTexture(GL_TEXTURE0+texUnit);
    glBindTexture(GL_TEXTURE_2D,texBufferObject);
    glBindSampler(texUnit,sampler);

    GLboolean isSampler=glIsSampler(sampler);
    LOGI("glIsSampler:sampler=%d\n",sampler);

    glEnableVertexAttribArray(iLocPosition);
    glEnableVertexAttribArray(iTexPosition);
    glVertexAttribPointer(iLocPosition, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(iTexPosition, 2, GL_FLOAT, GL_FALSE, 0, texcoords);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Sample::test() {
    setupGraphics(WINDOW_W, WINDOW_H);
    for(int i=0; i<ARRAY_NUM(factors); i++) {
        renderFrame(factors[i]);
        string mode_name = get_enum_name(factors[i]);
        string bmp_file  = filename + "-" + mode_name + ".bmp";
        LOGI("bmp_file=%s", bmp_file.c_str());
        save2bmp(bmp_file.c_str());
        Sample::SwapBuffers();
    }
    glDeleteSamplers(1,&sampler);
}

int main(int argc, char **argv) {
    Sample sample(__FILE__);

    sample.test();

    return 0;
}
