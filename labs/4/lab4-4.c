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
#include "object.h"
#include "terrain.h"

const int quadPixelWidth = 1;
const int quadPixelHeight = 1;

const float k_a = 0.1;
const float k_s = 0.8;
const float k_d = 0.6;

const char *terrainTexturePath = "textures/rutor.tga";
const char *terrainHeightMapPath = "textures/fft-terrain.tga";

// Objects/Models
Model *terrain;
Object groundSphere;
Object octagon;

// Shaders
GLuint pTerrain;
GLuint pPhong;

// HeightMap
TextureData ttex; 

void initShaders(void)
{
    pPhong = loadShaders("4/phong.vert", "4/phong.frag");
    pTerrain = loadShaders("4/terrain.vert", "4/terrain.frag");
    //pPhongTexture = loadShaders("3/phong-texture.vert", "3/phong-texture.frag");
    //pPhongMultiTexture = loadShaders("3/phong-multi-texture.vert", "3/phong-multi-texture.frag");

    /* Light */
    GLint isDirectional[] = {false, false, false, true};
    GLfloat lightPosition[] = {
        10.0f, 5.0f, 0.0f, // 1
        0.0f, 5.0f, 10.0f, // 2
        -1.0f, 0.0f, 0.0f, // 3
        1.0f, 1.0f, 1.0f  // 4
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
    // pTerrain
    glUseProgram(pTerrain);
    glUniform3fv(glGetUniformLocation(pTerrain, "lightPosition"), 4, lightPosition);
    glUniform3fv(glGetUniformLocation(pTerrain, "lightColor"), 4, lightColor);
    glUniform1iv(glGetUniformLocation(pTerrain, "isDirectional"), 4, isDirectional);
    glUniformMatrix4fv(glGetUniformLocation(pTerrain, "project"), 1, GL_TRUE, project.m);
    glUniform1f(glGetUniformLocation(pTerrain, "k_a"), k_a);
    glUniform1f(glGetUniformLocation(pTerrain, "k_d"), k_d);
    glUniform1f(glGetUniformLocation(pTerrain, "k_s"), k_s);
    
    GLuint terrainTexture;
    LoadTGATextureSimple(terrainTexturePath, &terrainTexture);
    glActiveTexture(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, terrainTexture);
    glUniform1i(glGetUniformLocation(pTerrain, "texUnit"), 0);

    // pPhong
    glUseProgram(pPhong);
    glUniform3fv(glGetUniformLocation(pPhong, "lightPosition"), 4, lightPosition);
    glUniform3fv(glGetUniformLocation(pPhong, "lightColor"), 4, lightColor);
    glUniform1iv(glGetUniformLocation(pPhong, "isDirectional"), 4, isDirectional);
    glUniformMatrix4fv(glGetUniformLocation(pPhong, "project"), 1, GL_TRUE, project.m);
    glUniform1f(glGetUniformLocation(pPhong, "k_a"), k_a);
    glUniform1f(glGetUniformLocation(pPhong, "k_d"), k_d);
    glUniform1f(glGetUniformLocation(pPhong, "k_s"), k_s);

    printError("init shader");
}

void init(void)
{
    // GL inits
    glClearColor(0.2, 0.2, 0.5, 0);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    printError("GL inits");
    // Camera
    vec3 cam = {0, 5, 0};
    vec3 lookAtPoint = {0, 0, 0};
    vec3 up = {0, 1, 0};
    cameraInit(cam, lookAtPoint, up);
    // Input
    inputInit();
    // Shaders
    initShaders();
    // Load terrain
    LoadTGATextureData("textures/fft-terrain.tga", &ttex);
    terrain = generateTerrain(&ttex, quadPixelWidth, quadPixelHeight);
    printError("init terrain");

    // Objects
    groundSphere = objectGet(
        "models/groundsphere.obj",
        NULL,
        NULL,
        NULL);
}

void display(void)
{
    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    printError("pre display");

    mat4 view = cameraLookAt();

    /* --------------- *
     * pTerrain shader *
     * --------------- */
    glUseProgram(pTerrain);
    // Terrain
    mat4 model = IdentityMatrix();
    shaderUpload(pTerrain, &model, &view, &camera.pos, -1, 10);	
    DrawModel(terrain, pTerrain, "inVertex", "inNormal", "inTexCoord");
    printError("display 2");

    /* ------------- *
     * pPhong shader *
     * ------------- */ 
    glUseProgram(pPhong);
    // Ground Sphere
    model = objectGetModelMatrix(&groundSphere);
    shaderUpload(pPhong, &model, &view, &camera.pos, -1, 125);
    DrawModel(groundSphere.model, pPhong, "inVertex", "inNormal", NULL);
    printError("display 3");

    glutSwapBuffers();
}

void timer(int i)
{
    mouseWarp(100, 100);
    keyboardHandler(&groundSphere);
    groundSphere.position.y = calcY(terrain->vertexArray, groundSphere.position.x, groundSphere.position.z, quadPixelWidth, quadPixelHeight, ttex.width, ttex.height);
    glutTimerFunc(20, &timer, i);
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitContextVersion(3, 2);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Lab 4.4");
    glutDisplayFunc(display);
    init();
    glutTimerFunc(20, &timer, 0);
    glutMainLoop();
    exit(0);
}
