#ifdef __APPLE__
#include <OpenGL/gl3.h>
// Linking hint for Lightweight IDE
// uses framework Cocoa
#endif
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include <math.h>
#include <stdio.h>
// Own
#include "transform.h"

// Globals
GLfloat vertices[] = {-0.5f, -0.5f, 0.0f,
                      -0.5f, 0.5f, 0.0f,
                      0.5f, -0.5f, 0.0f};

GLfloat colors[] = {1.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 1.0f};

// vertex array object
unsigned int vertexArrayObjID;
// program
GLuint program;

void OnTimer(int value)
{
    GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
    float a = 2 * M_PI / 2000;
    GLfloat rotZ[16];
    genRotZ(rotZ, a*t);
    glUniformMatrix4fv(glGetUniformLocation(program, "rotZ"), 1, GL_TRUE, rotZ);
    glutPostRedisplay();
    glutTimerFunc(20, &OnTimer, value);
}

void init(void)
{
    // vertex buffer object, used for uploading the geometry
    unsigned int vertexBufferObjID[2];
    
    dumpInfo();

    // GL inits
    glClearColor(0.2, 0.2, 0.5, 0);
    glDisable(GL_DEPTH_TEST);
    printError("GL inits");

    // Load and compile shader
    program = loadShaders("1/lab1-4.vert", "1/lab1-4.frag");
    printError("init shader");

    // Allocate and activate VAO
    glGenVertexArrays(1, &vertexArrayObjID);
    glBindVertexArray(vertexArrayObjID);
    
    // Allocate VBOs
    glGenBuffers(2, vertexBufferObjID);

    // VBO for vertex data
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID[0]);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(program, "inPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(program, "inPosition"));

    // VBO for color data
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID[1]);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(program, "inColor"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(program, "inColor"));

    // End of upload of geometry

    printError("init arrays");
}

void display(void)
{
    printError("pre display");

    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(vertexArrayObjID); // Select VAO
    glDrawArrays(GL_TRIANGLES, 0, 3);	// draw object

    printError("display");

    glFinish();
    //glutSwapBuffers();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitContextVersion(3, 2);
    glutCreateWindow("GL3 white triangle example");
    glutTimerFunc(20, &OnTimer, 0);
    glutDisplayFunc(display);
    init();
    glutMainLoop();
    return 0;
}
