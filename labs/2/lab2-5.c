#ifdef __APPLE__
#include <OpenGL/gl3.h>
#endif
// Common
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include "VectorUtils3.h"
// Own
#include "transform.h"

// VAO reference
unsigned int vertexArrayObjID;
// Shader program reference
GLuint program;
// Model
Model *m;

void init(void)
{
    // Load model
    m = LoadModel("models/bunnyplus.obj");

    // Load and compile shader
    program = loadShaders("2/lab2-5.vert", "2/lab2-5.frag");
    printError("init shader");

    // Print setup information
    dumpInfo();

    // Allocate and activate VAO
    glGenVertexArrays(1, &vertexArrayObjID);
    glBindVertexArray(vertexArrayObjID);

    // VBO references
    unsigned int vertexBufferObjID;
    unsigned int indexBufferObjID;
    unsigned int normalBufferObjID;

    // Allocate VBOs
    glGenBuffers(1, &vertexBufferObjID);
    glGenBuffers(1, &normalBufferObjID);
    glGenBuffers(1, &indexBufferObjID);
    printError("gen buffers");

    // VBO for vertex datab
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
    glBufferData(GL_ARRAY_BUFFER, m->numVertices * 3 * sizeof(GLfloat), m->vertexArray, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(program, "inPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(program, "inPosition"));
    printError("init vbo vertex");

    // VBO for normal data
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferObjID);
    glBufferData(GL_ARRAY_BUFFER, m->numVertices * 3 * sizeof(GLfloat), m->normalArray, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(program, "inNormal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(program, "inNormal"));
    printError("init vbo normal");

    // VBO for index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObjID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->numIndices * sizeof(GLuint), m->indexArray, GL_STATIC_DRAW);
    printError("init vbo index");

    // GL inits
    glClearColor(0.2, 0.2, 0.5, 0);
    glEnable(GL_DEPTH_TEST);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    printError("init gl inits");
}

void display(void)
{
    printError("display pre");
    // Clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw VAO
    glBindVertexArray(vertexArrayObjID);
    glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L);
    
    glFinish();
    //glutSwapBuffers();
    printError("display after");
}

void onTimer(int value)
{
    // Angular velocity
    float a_vel = M_PI / 5000;
    GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
    
    // Projection
    vec3 camera_pos = {1.0f, 0.0f, 0.0f};
    vec3 camera_look = {0.0f, 0.0f, 0.0f};
    vec3 camera_up = {0.0f, -1.0f, -1.0};
    mat4 project = lookAtv(camera_pos, camera_look, camera_up);

    // Apply rotations/translations
    mat4 transform = Ry(a_vel*t);

    // Upload
    glUniformMatrix4fv(glGetUniformLocation(program, "transform"), 1, GL_TRUE, transform.m);
    glUniformMatrix4fv(glGetUniformLocation(program, "project"), 1, GL_TRUE, project.m);

    printError("onTimer upload");

    // Redraw
    glutPostRedisplay();

    // Set timer
    glutTimerFunc(20, &onTimer, value);
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitContextVersion(3, 2);
    glutCreateWindow("Lab 2.5");
    glutTimerFunc(20, &onTimer, 0);
    glutDisplayFunc(display);
    init();
    glutMainLoop();
    return 0;
}
