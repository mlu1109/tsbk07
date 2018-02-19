#ifdef __APPLE__
#include <OpenGL/gl3.h>
// Linking hint for Lightweight IDE
// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"
// Own
#include "algebra.h"
#include "camera.h"
#include "input.h"
#include "shaders.h"
#include "terrain.h"
mat4 projectionMatrix;

const int quadPixelWidth = 1;
const int quadPixelHeight = 1;

const float k_a = 0.1;
const float k_s = 0.8;
const float k_d = 0.6;

// vertex array object
Model *m, *m2, *terrain;
// Reference to shader program
GLuint program;
GLuint tex1, tex2;
TextureData ttex; // terraiN

void initShaders(void)
{
    program = loadShaders("4/terrain.vert", "4/terrain.frag");
    //pPhongTexture = loadShaders("3/phong-texture.vert", "3/phong-texture.frag");
    //pPhongMultiTexture = loadShaders("3/phong-multi-texture.vert", "3/phong-multi-texture.frag");

    /* Light */
    GLint isDirectional[] = {false, false, false, true};
    GLfloat lightPosition[] = {
        10.0f, 5.0f, 0.0f, // 1
        0.0f, 5.0f, 10.0f, // 2
        -1.0f, 0.0f, 0.0f, // 3
        1.0f, 1.0f, 1.0f   // 4
    };
    GLfloat lightColor[] = {
        //0.0f, 0.0f, 0.0f,
        //0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, // 1: Red
        0.0f, 0.0f, 0.0f, // 2: Green
        0.0f, 0.0f, 0.0f, // 3: Blue
        1.0f, 1.0f, 1.0f  // 4: White
    };

    /* Projection */
    mat4 project = frustum(
        -0.1f, // Left
        0.1f,  // Right
        -0.1f, // Bottom
        0.1f,  // Top
        0.2f,  // Near
        300.0f // Far
    );

    /* Shaders */
    // program
    glUseProgram(program);
    glUniform3fv(glGetUniformLocation(program, "lightPosition"), 4, lightPosition);
    glUniform3fv(glGetUniformLocation(program, "lightColor"), 4, lightColor);
    glUniform1iv(glGetUniformLocation(program, "isDirectional"), 4, isDirectional);
    glUniformMatrix4fv(glGetUniformLocation(program, "project"), 1, GL_TRUE, project.m);
    glUniform1f(glGetUniformLocation(program, "k_a"), k_a);
    glUniform1f(glGetUniformLocation(program, "k_d"), k_d);
    glUniform1f(glGetUniformLocation(program, "k_s"), k_s);

    GLuint terrainTexture;
    LoadTGATextureSimple("textures/maskros512.tga", &terrainTexture);
    glActiveTexture(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, terrainTexture);
    glUniform1i(glGetUniformLocation(program, "texUnit"), 0);
    printError("init shaders");
}

void init(void)
{
    // GL inits
    glClearColor(0.2, 0.2, 0.5, 0);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    printError("GL inits");
    // Camera
    vec3 cam = {0, 5, 8};
    vec3 lookAtPoint = {2, 0, 2};
    vec3 up = {0, 1, 0};
    cameraInit(cam, lookAtPoint, up);
    // Input
    inputInit();

    projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 300.0);

    // Shaders
    initShaders();

    // Load terrain data
    LoadTGATextureData("textures/fft-terrain.tga", &ttex);
    terrain = generateTerrain(&ttex, quadPixelWidth, quadPixelHeight);
    printError("init terrain");
}

void display(void)
{
    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    printError("pre display");

    mat4 view = cameraLookAt();

    /* --------------- *
     * program shader *
     * --------------- */
    glUseProgram(program);
    // Terrain
    mat4 model = IdentityMatrix();
    shaderUpload(program, &model, &view, &camera.pos, 0, 10);
    DrawModel(terrain, program, "inVertex", "inNormal", "inTexCoord");
    printError("display 2");

    glutSwapBuffers();
}

void timer(int i)
{
    mouseWarp(100, 100);
    keyboardHandler(NULL);
    glutTimerFunc(20, &timer, i);
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitContextVersion(3, 2);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Lab 4.3");
    glutDisplayFunc(display);
    init();
    glutTimerFunc(20, &timer, 0);
    glutMainLoop();
    exit(0);
}
