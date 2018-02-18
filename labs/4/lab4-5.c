#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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

const float k_a = 0.3;
const float k_s = 0.8;
const float k_d = 0.2;

const char *terrainTexturePath = "textures/rutor.tga";
const char *sphereTexturePath = "textures/conc.tga";
const char *terrainHeightMapPath = "textures/fft-terrain.tga";
const char *objectsTexturePath = "textures/dirt.tga";

// Objects/Models
Model *terrain;
Model *water;
Model *square;
Object sphere;
const int numObjects = 20;
Object objects[100];

// Shaders
GLuint pTerrain;
GLuint pPhong;
GLuint pPhongTexture;

// HeightMap
TextureData ttex;

void initShaders(void)
{
    pPhong = loadShaders("4/phong.vert", "4/phong.frag");
    pTerrain = loadShaders("4/terrain.vert", "4/terrain.frag");
    pPhongTexture = loadShaders("4/phong-texture.vert", "4/phong-texture.frag");
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
        1.0f, 0.0f, 0.0f, // 1: Red
        0.0f, 1.0f, 0.0f, // 2: Green
        0.0f, 0.0f, 1.0f, // 3: Blue
        1.0f, 1.0f, 1.0f  // 4: White
    };

    /* Projection */
    mat4 project = frustum(
        -0.1f,  // Left
        0.1f,   // Right
        -0.1f,  // Bottom
        0.1f,   // Top
        0.2f,   // Near
        1000.0f // Far
    );

    /* Shaders */
    // pTerrain
    glUseProgram(pTerrain);
    glUniformMatrix4fv(glGetUniformLocation(pTerrain, "project"), 1, GL_TRUE, project.m);
    shaderLight(pTerrain, lightPosition, lightColor, isDirectional, &k_a, &k_d, &k_s);

    GLuint terrainTexture;
    LoadTGATextureSimple(terrainTexturePath, &terrainTexture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, terrainTexture);
    glUniform1i(glGetUniformLocation(pTerrain, "texUnit"), 0);
    printError("init pTerrain");

    // pPhong
    glUseProgram(pPhong);
    glUniformMatrix4fv(glGetUniformLocation(pPhong, "project"), 1, GL_TRUE, project.m);
    shaderLight(pPhong, lightPosition, lightColor, isDirectional, &k_a, &k_d, &k_s);
    printError("init pPhong");

    // pPhongTexture
    glUseProgram(pPhongTexture);
    glUniformMatrix4fv(glGetUniformLocation(pPhongTexture, "project"), 1, GL_TRUE, project.m);
    shaderLight(pPhongTexture, lightPosition, lightColor, isDirectional, &k_a, &k_d, &k_s);

    GLuint sphereTexture;
    glActiveTexture(GL_TEXTURE1);
    LoadTGATextureSimple(sphereTexturePath, &sphereTexture);
    glBindTexture(GL_TEXTURE_2D, sphereTexture);

    printError("init pPhongTexture");
}

void initModels(void)
{
    // Terrain
    LoadTGATextureData("textures/fft-terrain.tga", &ttex);
    terrain = generateTerrain(&ttex, quadPixelWidth, quadPixelHeight);
    // Sphere
    vec3 scale = {3, 3, 3};
    sphere = objectGet("models/groundsphere.obj", NULL, &scale, NULL, NULL);
    // Octagon
    int seed = time(NULL);
    srand(seed);
    for (int i = 0; i < numObjects; ++i)
    {
        int s = rand() % 10 + 1;
        scale = (vec3){s, s, s};
        vec3 position = {rand() % (quadPixelWidth * ttex.width), 0, rand() % (quadPixelWidth * ttex.height)};
        vec3 rotation = {0, rand(), 0};
        vec3 direction = {rand(), rand(), rand()};
        direction = Normalize(direction);
        const char *model_i = i % 2 == 0 ? "models/octagon.obj" : "models/groundsphere.obj";
        objects[i] = objectGet(model_i, &position, &scale, &rotation, &direction);
    }
    // Square
    int vertices = 4;
    int indices = 6;
    GLfloat vertexArray[] = {
        0, 0, 0,
        1, 0, 0,
        1, 0, 1,
        0, 0, 1};
    GLfloat normalArray[] = {
        1, 1, 1,
        1, 1, 1,
        1, 1, 1,
        1, 1, 1};
    GLuint indexArray[] = {
        1, 2, 3,
        1, 3, 4};
    square = LoadDataToModel(vertexArray, normalArray, NULL, NULL, indexArray, vertices, indices);
}

void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    printError("GL inits");
    vec3 cam = {0, 5, 0};
    vec3 lookAtPoint = {0, 0, 0};
    vec3 up = {0, 1, 0};
    cameraInit(cam, lookAtPoint, up);
    inputInit();
    initModels();
    initShaders();
    printError("init");
}

void display(void)
{
    GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
    // Enable blending
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    printError("pre display");

    mat4 view = cameraLookAt();

    /* --------------- *
     * pTerrain shader *
     * --------------- */
    glUseProgram(pTerrain);
    mat4 model = IdentityMatrix();
    shaderUpload(pTerrain, &model, &view, &camera.pos, 0, 10);
    DrawModel(terrain, pTerrain, "inVertex", "inNormal", "inTexCoord");
    printError("display 2");
    /* -------------------- *
     * pPhongTexture shader *
     * ---------------------*/
    glUseProgram(pPhongTexture);
    // Ground Sphere
    model = objectGetModelMatrix(&sphere);
    shaderUpload(pPhongTexture, &model, &view, &camera.pos, 1, 125);
    DrawModel(sphere.model, pPhongTexture, "inVertex", "inNormal", "inTexCoord");
    /* ------------- *
     * pPhong shader *
     * ------------- */
    glUseProgram(pPhong);
    vec3 color;
    // Random objects

    for (int i = 0; i < numObjects; ++i)
    {
        color = Normalize((vec3) {i % 3, (i + 1) % 3, (i + 2) % 3});
    glUniform3fv(glGetUniformLocation(pPhong, "color"), 1, &color.x);

        model = objectGetModelMatrix(&objects[i]);
        shaderUpload(pPhong, &model, NULL, NULL, 1, 500);
        DrawModel(objects[i].model, pPhong, "inVertex", "inNormal", "inTexCoord");
    }
    // Water (Blending Enabled)
    color = (vec3){0, 1, 1};
    glUniform3fv(glGetUniformLocation(pPhong, "color"), 1, &color.x);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    model = mult2(T(0, 4, 0), S(ttex.w * quadPixelWidth, 0, ttex.h * quadPixelHeight));
    shaderUpload(pPhong, &model, &view, &camera.pos, -1, 10);
    glUniform1f(glGetUniformLocation(pPhong, "t"), t);
    DrawModel(square, pPhong, "inVertex", "inNormal", NULL);
    glDisable(GL_BLEND);
    // Blendin disabled
    printError("display");
    glutSwapBuffers();
}

void timer(int i)
{
    mouseWarp(100, 100);
    keyboardHandler(&sphere);
    GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
    sphere.position.y = calcY(terrain->vertexArray, sphere.position.x, sphere.position.z, quadPixelWidth, quadPixelHeight, ttex.width, ttex.height);
    for (int i = 0; i < numObjects; ++i)
    {
        Object *o = &objects[i];
        o->position.y = calcY(terrain->vertexArray, o->position.x, o->position.z, quadPixelWidth, quadPixelHeight, ttex.width, ttex.height);
        o->rotation.y = M_PI / 3000 * t;
        objectMoveInDirection(o, 1);
        // Keep inside map
        if (o->position.x <= 0 || o->position.x >= quadPixelWidth * ttex.width || o->position.z <= 0 || o->position.z >= quadPixelHeight * ttex.height)
        {
            o->direction = ScalarMult(o->direction, -1);
            objectMoveInDirection(o, 2);
        }
    }
    for (int i = 0; i < numObjects - 1; ++i)
    {
        Object *a = &objects[i];
        for (int j = i + 1; j < numObjects; ++j)
        {
            Object *b = &objects[j];
            objectHandleCollision(a, b);
        }
        objectHandleCollision(a, &sphere);
    }
    glutTimerFunc(20, &timer, i);
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitContextVersion(3, 2);
    glutInitWindowSize(1000, 1000);
    glutCreateWindow("Lab 4.5");
    glutDisplayFunc(display);
    init();
    glutTimerFunc(20, &timer, 0);
    glutMainLoop();
    exit(0);
}
