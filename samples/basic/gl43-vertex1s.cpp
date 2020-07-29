/* =================================================================
 * Copyright (c) 2019 Botson Corp
 * Botson Confidential and Proprietary
 * * =================================================================
*/

/**
 * @file points.cpp
 * @author Otto.Liu
 * @par History:
 *    A001: Otto.Liu, 2019-08-26, Add commit
 * @brief basic test for points related API
 * @par TestPoints:
 *
 */

#include "Sample.h"
#define BUFFER_OFFSET(x)  ((const void*) (x))
using std::string;
using namespace BotsonSDK;

const float vertices[] = {
    -0.4f, -0.4f, 0.0f, 1.0,
    -0.4f,  0.0f, 0.0f, 1.0,
    -0.4f,  0.4f, 0.0f, 1.0,
     0.0f, -0.4f, 0.0f, 1.0,
     0.0f,  0.0f, 0.0f, 1.0,
     0.0f,  0.4f, 0.0f, 1.0,
     0.4f, -0.4f, 0.0f, 1.0,
     0.4f,  0.0f, 0.0f, 1.0,
     0.4f,  0.4f, 0.0f, 1.0,
     0.8f,  0.8f, 0.0f, 1.0,
    -0.8f, -0.8f, 0.0f, 1.0,
};


GLshort color[] = {
    0, 0, 0, 1,
    0, 0, 0, 1,
    0, 0, 0, 1,
    0, 0, 0, 1,
    0, 0, 0, 1,
    0, 0, 0, 1,
    0, 0, 0, 1,
    0, 0, 0, 1,
    0, 0, 0, 1,
    0, 0, 0, 1,
    0, 0, 0, 1
};


GLuint programID, VAOs, Buffers;

bool setupGraphics(int w, int h) {
    string vertexShaderPath = ((string)GLSL_PATH) + "vertex1s.vert";
    string fragmentShaderPath = ((string)GLSL_PATH) + "vertex1s.frag";

    programID = GL_CHECK(Shader::CreateProgram(vertexShaderPath.c_str(), fragmentShaderPath.c_str()));
    GL_CHECK(glUseProgram(programID));

    GL_CHECK(glGenVertexArrays(1, &VAOs));
    GL_CHECK(glBindVertexArray(VAOs));

    GL_CHECK(glGenBuffers(1, &Buffers));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, Buffers));

    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(color), NULL, GL_STATIC_DRAW));
    GL_CHECK(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices));
    GL_CHECK(glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(color), color));
    GL_CHECK(glEnableVertexAttribArray(0));
    GL_CHECK(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0));
    GL_CHECK(glEnableVertexAttribArray(1));
    GL_CHECK(glVertexAttribPointer(1, 4, GL_SHORT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertices))));

    GL_CHECK(glClearColor(1.0, 1.0, 1.0, 1.0));
    GL_CHECK(glViewport(0, 0, w, h));

    return true;
}

void renderFrame(void) {
    GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    GL_CHECK(glUseProgram(programID));
    GL_CHECK(glPointSize(10.0));
    GL_CHECK(glBindVertexArray(VAOs));

    GL_CHECK(glEnableVertexAttribArray(1));
    GL_CHECK(glDrawArrays(GL_POINTS, 0, 1));

    GL_CHECK(glVertexAttrib4s(1, 1, 1, 0, 1));
    GL_CHECK(glDisableVertexAttribArray(1));
    GL_CHECK(glDrawArrays(GL_POINTS, 1, 1));

    const GLshort col[4] = {1, 1, 0, 1};
    GL_CHECK(glVertexAttrib4sv(1, &col[0]));
    GL_CHECK(glDisableVertexAttribArray(1));
    GL_CHECK(glDrawArrays(GL_POINTS, 2, 1));

    GL_CHECK(glVertexAttrib3s(1, 1, 0, 1));
    GL_CHECK(glDisableVertexAttribArray(1));
    GL_CHECK(glDrawArrays(GL_POINTS, 3, 1));

    const GLshort col1[3] = {1, 0, 1};
    GL_CHECK(glVertexAttrib3sv(1, &col1[0]));
    GL_CHECK(glDisableVertexAttribArray(1));
    GL_CHECK(glDrawArrays(GL_POINTS, 4, 1));

    GL_CHECK(glVertexAttrib2s(1, 0, 1));
    GL_CHECK(glDisableVertexAttribArray(1));
    GL_CHECK(glDrawArrays(GL_POINTS, 5, 1));

    const GLshort col2[2] = {0, 1};
    GL_CHECK(glVertexAttrib2sv(1, &col2[0]));
    GL_CHECK(glDisableVertexAttribArray(1));
    GL_CHECK(glDrawArrays(GL_POINTS, 6, 1));

    GL_CHECK(glVertexAttrib1s(1, 1));
    GL_CHECK(glDisableVertexAttribArray(1));
    GL_CHECK(glDrawArrays(GL_POINTS, 7, 1));

    const GLshort col3[1] = {1};
    GL_CHECK(glVertexAttrib1sv(1, col3));
    GL_CHECK(glDisableVertexAttribArray(1));
    GL_CHECK(glDrawArrays(GL_POINTS, 8, 1));

    const GLshort col4[4] = {1, 1, 0, 1};
    GL_CHECK(glVertexAttrib4Nsv(1, col4));
    GL_CHECK(glDisableVertexAttribArray(1));
    GL_CHECK(glDrawArrays(GL_POINTS, 9, 1));

    const GLshort col5[4] = {1, 1, 0, 1};
    GL_CHECK(glVertexAttribI4sv(1, col5));
    GL_CHECK(glDisableVertexAttribArray(1));
    GL_CHECK(glDrawArrays(GL_POINTS, 10, 1));
}

void Sample::test() {
    setupGraphics(WINDOW_W, WINDOW_H);
    renderFrame();
    string bmp_file = filename + ".bmp";
    LOGI("bmp_file=%s",bmp_file.c_str());
    save2bmp(bmp_file.c_str());
    Sample::SwapBuffers();
}

int main(int argc, char **argv) {
    Sample sample(__FILE__, GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE, argc,argv);

    sample.test();

    return 0;
}
