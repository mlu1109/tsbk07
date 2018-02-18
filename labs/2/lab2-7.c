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
// Models
Model *m[2];

void init(void)
{
    // Load models
    m[0] = LoadModelPlus("models/bunnyplus.obj");
    m[1] = LoadModelPlus("models/cubeplus.obj");

    // Load and compile shader
    program = loadShaders("2/lab2-7.vert", "2/lab2-7.frag");
    printError("init shader");

    // Print setup information
    dumpInfo();

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

    // View
    float a_vel = M_PI / 2500;
    GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
    float radius = 6.0f;
    // Orbit in the zy-plane around camera_look with radius
    vec3 camera_pos = {cos(a_vel * t) * radius, 0.0f, -5.0f + sin(a_vel * t) * radius};
    vec3 camera_look = {0.0f, 0.0f, -5.0f};
    vec3 camera_up = {0.0f, 1.0f, 0.0f};
    mat4 view = lookAtv(camera_pos, camera_look, camera_up);

    // Projection
    GLfloat project[16];
    float near = 1.0f;
    float far = 30.0f;
    float right = 0.5f;
    float left = -0.5f;
    float top = 0.5f;
    float bottom = -0.5f;
    genProj(project, near, far, right, left, top, bottom);

    // Transformations
    mat4 transform[2];
    transform[0] = T(-1.0f, 0.0f, -5.0f);
    transform[1] = Mult(T(1.0f, 0.0f, -5.0f), Ry(M_PI));

    // Shared
    glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, view.m);
    glUniformMatrix4fv(glGetUniformLocation(program, "project"), 1, GL_TRUE, project);

    // Model 0
    glUniformMatrix4fv(glGetUniformLocation(program, "transform"), 1, GL_TRUE, transform[0].m);
    DrawModel(m[0], program, "inPosition", "inNormal", NULL);

    // Model 1
    glUniformMatrix4fv(glGetUniformLocation(program, "transform"), 1, GL_TRUE, transform[1].m);
    DrawModel(m[1], program, "inPosition", "inNormal", NULL);

    // Done
    printError("display upload");
    glFinish();
    //glutSwapBuffers();
    printError("display after");
}

void onTimer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(20, &onTimer, value);
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitContextVersion(3, 2);
    glutCreateWindow("Lab 2.7");
    glutTimerFunc(20, &onTimer, 0);
    glutDisplayFunc(display);
    init();
    glutMainLoop();
    return 0;
}
