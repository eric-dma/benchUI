/**
 * @file gl14-multidrawelements.cpp
 * @testpoint glMultiDrawElements
 *
 */
#include "gl-util.h"
unsigned int DisplayMode = GLUT_SINGLE | GLUT_RGBA;

static const GLenum factors[] = {
	GL_POINTS,
	GL_LINE_STRIP,
	GL_LINE_LOOP,
	GL_LINES,
	GL_TRIANGLE_STRIP,
	GL_TRIANGLE_FAN,
	GL_TRIANGLES,
	GL_QUAD_STRIP,
	GL_QUADS,
	GL_POLYGON 
};

static const GLenum types[] = {
GL_UNSIGNED_BYTE,
GL_UNSIGNED_SHORT,
GL_UNSIGNED_INT
};

static const GLint vertices [] = {-4,-2,  -4,2,
				  0,2,  0,-2,   
				4,-2,  4,2};
static const GLfloat color[] = {0.0, 1.0, 0.0,
				1.0, 0.0, 0.0,
				0.0, 0.0, 1.0,
				0.0, 1.0, 0.0,
				1.0, 0.0, 0.0,
				0.0, 0.0, 1.0};

void setupGraphics(int width, int height)
{
    glClearColor(0.0,0.0,0.0,0.0);
    glMatrixMode(GL_PROJECTION);
    glOrtho(-5,5,-5,5,5,15);
    glMatrixMode(GL_MODELVIEW);
    gluLookAt(0,0,10,0,0,0,0,1,0);
}

void renderFrame(GLenum factor, GLenum type)
{
	GLsizei cout[] = {4,4};
	GLubyte indicesUp_ub[] = {0,1,2,3};
	GLubyte indicesDown_ub[] = {2,3,4,5};
	GLvoid* indices_ub[2] = {indicesUp_ub,indicesDown_ub};
	GLushort indicesUp_us[] = {0,1,2,3};
	GLushort indicesDown_us[] = {2,3,4,5};
	GLvoid* indices_us[2] = {indicesUp_us,indicesDown_us};
	GLuint indicesUp_ui[] = {0,1,2,3};
	GLuint indicesDown_ui[] = {2,3,4,5};
	GLvoid* indices_ui[2] = {indicesUp_ui,indicesDown_ui};

	glClear(GL_COLOR_BUFFER_BIT);
	glLineWidth(2.0);
	glPointSize(4.0);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glColorPointer(3, GL_FLOAT, 0, color);
	glVertexPointer(2, GL_INT, 0, vertices);
    if(type == GL_UNSIGNED_BYTE)
	glMultiDrawElements(factor, cout, GL_UNSIGNED_BYTE, indices_ub,2);
    if(type == GL_UNSIGNED_SHORT)
	glMultiDrawElements(factor, cout, GL_UNSIGNED_SHORT, indices_us,2);
    if(type == GL_UNSIGNED_INT)
	glMultiDrawElements(factor, cout, GL_UNSIGNED_INT, indices_ui,2);
	glFlush();
}

int main(int argc, char** argv)
{
    string file = "gl14-multidrawelements";
    GBtest_init(&argc,argv,file,DisplayMode);
    setupGraphics(WINDOW_W, WINDOW_H);
    for(int i=0; i<ARRAY_NUM(factors); i++) {
    for(int j=0; j<ARRAY_NUM(types); j++) {
            string src_name = get_primitives_name(factors[i]);
            string type_name = get_enum_name(types[j]);
	    string filename = file + "-" + src_name + "-" + type_name + ".bmp";
	    LOGIF("[%d_%d]: filename=%s\n", i, j, filename.c_str());
            renderFrame(factors[i],types[j]);
	    save2bmp(filename.c_str());
	    glutSwapBuffers();
    }}
    GBtest_deinit();

    return 0;
}


