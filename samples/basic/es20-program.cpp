/* =================================================================
 * Copyright (c) 2019 Botson Corp
 * Botson Confidential and Proprietary
 * * =================================================================
*/

/**
 * @file program.cpp
 * @author Otto.Liu
 * @par History:
 *    A001: Otto.Liu, 2019-08-26, Add commit
 * @brief basic test for program related API
 * @par TestPoints:
 *
 */

#include "Sample.h"

using std::string;
using namespace BotsonSDK;

const float triangleVertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f,
     0.0f, -0.5f, 0.0f
};

const float triangleVerticesLittle[] = {
    -0.45f, -0.45f, 0.0f,
     0.45f, -0.45f, 0.0f,
     0.0f,  0.45f, 0.0f,
     0.0f, -0.45f, 0.0f
};

const float triangleColors[] = {
    1.0, 0.0, 0.0, 1.0,
    0.0, 1.0, 0.0, 1.0,
    0.0, 1.0, 0.0, 1.0,
    0.0, 1.0, 0.0, 1.0
};

const GLubyte indices[] = {0,1,2};
const GLubyte subdata[] = {1,3};

const float triangleVertices2[] = {
     0.0f,-0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f,
     0.5f, 0.5f, 0.0f,
};

const float triangleColors2[] = {
    1.0, 0.0, 0.0, 1.0,
    0.0, 1.0, 0.0, 1.0,
    0.0, 1.0, 0.0, 1.0,
};

GLuint programID;
GLint iLocPosition = -1;
GLint iLocFillColor = -1;
GLuint vertexShaderID = 0;
GLuint fragmentShaderID = 0;

char* loadS(const char *filename) {
        FILE *file = fopen(filename, "rb");
        if(file == NULL) {
            LOGI("Cannot read file '%s'\n", filename);
            exit(1);
        }

        fseek(file, 0, SEEK_END);

        long length = ftell(file);

        fseek(file, 0, SEEK_SET);
        char *shader = (char *)calloc(length + 1, sizeof(char));
        if(shader == NULL) {
            LOGE("Out of memory at %s:%i\n", __FILE__, __LINE__);
            exit(1);
        }

        size_t numberOfBytesRead = fread(shader, sizeof(char), length, file);
        if (numberOfBytesRead != length) {
            LOGE("Error reading %s (read %d of %d)", filename, numberOfBytesRead, length);
            exit(1);
        }

        shader[length] = '\0';
        fclose(file);

        return shader;
}

void procShader(GLuint *shader, const char *filename, GLint shaderType) {
        char *strings[1] = { NULL };

        *shader = GL_CHECK(glCreateShader(shaderType));
        strings[0] = loadS(filename);  ////////////////
        GL_CHECK(glShaderSource(*shader, 1, strings, NULL));

        free((void *)(strings[0]));
        strings[0] = NULL;

        GL_CHECK(glCompileShader(*shader));
        GLint status;
        GL_CHECK(glGetShaderiv(*shader, GL_COMPILE_STATUS, &status));

        if(status != GL_TRUE) {
            GLint length;
            char *debugSource = NULL;
            char *errorLog = NULL;

            GL_CHECK(glGetShaderiv(*shader, GL_SHADER_SOURCE_LENGTH, &length));
            debugSource = (char *)malloc(length);
            GL_CHECK(glGetShaderSource(*shader, length, NULL, debugSource));
            LOGI("Debug source START:\n%s\nDebug source END\n\n", debugSource);
            free(debugSource);

            GL_CHECK(glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &length));
            errorLog = (char *)malloc(length);
            GL_CHECK(glGetShaderInfoLog(*shader, length, NULL, errorLog));
            LOGI("Log START:\n%s\nLog END\n\n", errorLog);
            free(errorLog);

            LOGE("Compilation FAILED!\n\n");
            exit(1);
        }
}

bool setupGraphics(int width, int height) {
    GLboolean isP;

    string vertexShaderPath = ((string)GLSL_PATH) + "basic.vert"; 
    string fragmentShaderPath = ((string)GLSL_PATH) + "basic.frag";

    GL_CHECK(glEnable(GL_DEPTH_TEST));
    GL_CHECK(glDepthFunc(GL_LEQUAL));

    procShader(&vertexShaderID, vertexShaderPath.c_str(), GL_VERTEX_SHADER);
    procShader(&fragmentShaderID, fragmentShaderPath.c_str(), GL_FRAGMENT_SHADER);
    LOGI("fragmentShaderID = %d", fragmentShaderID);

    programID = GL_CHECK(glCreateProgram());
    isP=glIsProgram(programID);
    LOGI("glIsProgram %d=%d\n", programID, isP);
    if (programID == 0) {
        LOGE("Could not create program.");
        return false;
    }

    glValidateProgram(programID);
    GL_CHECK(glAttachShader(programID, vertexShaderID));
    GL_CHECK(glAttachShader(programID, fragmentShaderID));
    GL_CHECK(glLinkProgram(programID));
    GL_CHECK(glUseProgram(programID));

    iLocPosition = glGetAttribLocation(programID, "a_v4Position");
    iLocFillColor = glGetAttribLocation(programID, "a_v4FillColor");
    GLint iLocProjection = GL_CHECK(glGetUniformLocation(programID, "u_m4Projection"));
    if(iLocProjection == -1) {
        LOGW("Warning: Uniform not found at %s:%i\n", __FILE__, __LINE__);
    } else {
        GL_CHECK(glUniformMatrix4fv(iLocProjection, 1, GL_FALSE, Matrix::identityMatrix.getAsArray()));
    }

    GL_CHECK(glViewport(0, 0, width, height));
    GL_CHECK(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
    GL_CHECK(glClearDepthf(1.0f));

    return true;
}

void renderFrame(void) {
    GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    GL_CHECK(glUseProgram(programID));
    GL_CHECK(glVertexAttribPointer(iLocPosition, 3, GL_FLOAT, GL_FALSE, 0,
                                   triangleVertices));
    GL_CHECK(glEnableVertexAttribArray(iLocPosition));

    if(iLocFillColor != -1) {
        GL_CHECK(glVertexAttribPointer(iLocFillColor, 4, GL_FLOAT, GL_FALSE, 0, triangleColors));
        GL_CHECK(glEnableVertexAttribArray(iLocFillColor));
    }

    GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, 3));

    GLint tmp;
    GLboolean isP;
    GLsizei length,maxlength;
    GLchar *infoLog;
    glGetProgramiv(programID, GL_ACTIVE_ATTRIBUTES, &tmp);
    LOGI("glGetProgramiv(GL_ACTIVE_ATTRIBUTES) num=%d\n", tmp);

    glGetProgramiv(programID, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &tmp);
    LOGI("glGetProgramiv(GL_ACTIVE_ATTRIBUTE_MAX_LENGTH) length=%d\n", tmp);

    glGetProgramiv(programID, GL_LINK_STATUS, &tmp);
    LOGI("glGetProgramiv(GL_LINK_STATUS) status=%d\n", tmp);

    glGetProgramiv(programID, GL_VALIDATE_STATUS, &tmp);
    LOGI("glGetProgramiv(GL_VALIDATE_STATUS) status=%d\n", tmp);

    glGetProgramiv(programID, GL_DELETE_STATUS, &tmp);
    LOGI("glGetProgramiv(GL_DELETE_STATUS) status=%d\n", tmp);

    glGetProgramiv(programID, GL_ATTACHED_SHADERS, &tmp);
    LOGI("glGetProgramiv(GL_ATTACHED_SHADERS): %d\n", tmp);

    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &tmp);
    LOGI("glGetProgramiv(GL_INFO_LOG_LENGTH) maxlength=%d\n", tmp);
    infoLog = (char *)malloc(tmp+1);
    glGetProgramInfoLog(programID, tmp, &length, infoLog);
    LOGI("glGetProgramInfoLog:length=%d,infoLog=%s\n", length, infoLog);

    glDeleteProgram(programID);
    isP=glIsProgram(programID);
    LOGI("glIsProgram %d=%d\n", programID, isP);
    glGetProgramiv(programID, GL_DELETE_STATUS, &tmp);
    LOGI("glGetProgramiv(GL_DELETE_STATUS)%d=%d\n", programID, tmp);

    GLint parami;
    glGetIntegerv(GL_CURRENT_PROGRAM, &parami);
    LOGI("get(GL_CURRENT_PROGRAM): %d", parami);


    glDisableVertexAttribArray(iLocFillColor);
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
    Sample sample(__FILE__);

    sample.test();

    return 0;
}

