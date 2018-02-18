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

#include "MicroGlut.h"
#include "GL_utilities.h"
#include <math.h>
#include <stdio.h>
#include "loadobj.h"
// Own
#include "transform.h"

// Globals
// Data would normally be read from files
Model *m;

// vertex array object
unsigned int bunnyVertexArrayObjID;
// program
GLuint program;

void OnTimer(int value)
{
    GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
    float a = M_PI / 5000; // angular velocity
    GLfloat rotZ[16];
    genRotZ(rotZ, a*t);
    GLfloat rotY[16];
    genRotY(rotY, a*t);
    GLfloat rotX[16];
    genRotX(rotX, a*t);

    glUniformMatrix4fv(glGetUniformLocation(program, "rotZ"), 1, GL_TRUE, rotZ);
    glUniformMatrix4fv(glGetUniformLocation(program, "rotY"), 1, GL_TRUE, rotY);
    glUniformMatrix4fv(glGetUniformLocation(program, "rotX"), 1, GL_TRUE, rotX);
    printError("ontimer upload");

    glutPostRedisplay();
    glutTimerFunc(20, &OnTimer, value);
}

void init(void)
{
    // vertex buffer object, used for uploading the geometry
    unsigned int bunnyVertexBufferObjID;
    unsigned int bunnyIndexBufferObjID;
    unsigned int bunnyNormalBufferObjID;
    dumpInfo();

    // GL inits
    glClearColor(0.2, 0.2, 0.5, 0);
    glEnable(GL_DEPTH_TEST);
    printError("GL inits");

    // Load object
    m = LoadModel("models/bunny.obj");

    // Load and compile shader
    program = loadShaders("1/lab1-6.vert", "1/lab1-6.frag");
    printError("init shader");

    glBindVertexArray(bunnyVertexArrayObjID);

    glGenVertexArrays(1, &bunnyVertexArrayObjID);
    glGenBuffers(1, &bunnyVertexBufferObjID);
    glGenBuffers(1, &bunnyIndexBufferObjID);
    glGenBuffers(1, &bunnyNormalBufferObjID);
    glBindVertexArray(bunnyVertexArrayObjID);

    // VBO for vertex data
    glBindBuffer(GL_ARRAY_BUFFER, bunnyVertexBufferObjID);
    glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->vertexArray, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(program, "inPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
    glEnableVertexAttribArray(glGetAttribLocation(program, "inPosition"));

    // VBO for normal data
    glBindBuffer(GL_ARRAY_BUFFER, bunnyNormalBufferObjID);
    glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->normalArray, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(program, "inNormal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(program, "inNormal"));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bunnyIndexBufferObjID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->numIndices*sizeof(GLuint), m->indexArray, GL_STATIC_DRAW);
    printError("load models");

    // Z-buffer
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    printError("init arrays");
}

void display(void)
{
    printError("pre display");

    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(bunnyVertexArrayObjID);    // Select VAO
    glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L);

    printError("display");

    //glFinish();
    glutSwapBuffers();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitContextVersion(3, 2);
    glutCreateWindow("Lab 1.6");
    glutTimerFunc(20, &OnTimer, 0);
    glutDisplayFunc(display);
    init();
    glutMainLoop();
    return 0;
}
