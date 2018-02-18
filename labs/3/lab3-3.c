#ifdef __APPLE__
#include <OpenGL/gl3.h>
#endif
// Common
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "loadobj.h"
#include "VectorUtils3.h"
#include "LoadTGA.h"
// Own
#include "algebra.h"
#include "camera.h"
#include "input.h"

GLuint colorShaders;
GLuint textureShaders;

mat4 project;

struct WindMill
{
    Model *blades;
    Model *walls;
    Model *balcony;
    Model *roof;
} wm;

Model *skybox;
Model *cube;
Model *car;

void init(void)
{
    dumpInfo();
    colorShaders = loadShaders("3/normal.vert", "3/normal.frag");
    textureShaders = loadShaders("3/texture.vert", "3/texture.frag");
    printError("init shader");
    /* Init gl */
    glClearColor(0.2, 0.2, 0.5, 0);
    /* Init input */
    inputInit();
    /* Init models */
    // Windmill
    wm.blades = LoadModelPlus("models/windmill/blade.obj");
    wm.walls = LoadModelPlus("models/windmill/windmill-walls.obj");
    wm.balcony = LoadModelPlus("models/windmill/windmill-balcony.obj");
    wm.roof = LoadModelPlus("models/windmill/windmill-roof.obj");
    // Skybox
    skybox = LoadModelPlus("models/skybox.obj");
    // Cube (also used for ground)
    cube = LoadModelPlus("models/cubeplus.obj");
    // Venus
    car = LoadModelPlus("models/bilskiss.obj");
    /* Init textures */
    // Skybox
    glActiveTexture(GL_TEXTURE0);
    GLuint skyboxTexture;
    LoadTGATextureSimple("textures/SkyBox512.tga", &skyboxTexture);
    glBindTexture(GL_TEXTURE_2D, skyboxTexture);
    // Ground
    glActiveTexture(GL_TEXTURE1);
    GLuint groundTexture;
    LoadTGATextureSimple("textures/grass.tga", &groundTexture);
    glBindTexture(GL_TEXTURE_2D, groundTexture);
    // Car
    glActiveTexture(GL_TEXTURE2);
    GLuint carTexture;
    LoadTGATextureSimple("models/bilskissred.tga", &carTexture);
    glBindTexture(GL_TEXTURE_2D, carTexture);
    /* Init camera */
    vec3 camera_pos = {0.0f, 5.0f, -30.0f};
    vec3 camera_look = {0.0f, 5.0f, 0.0f};
    vec3 camera_up = {0.0f, 1.0f, 0.0f};
    cameraInit(camera_pos, camera_look, camera_up);
    /* Init projection */
    project = frustum(
        -0.5f, // Left
        0.5f,  // Right
        -0.5f, // Bottom
        0.5f,  // Top
        1.0f,  // Near
        300.0f // Far
    );
    glUseProgram(colorShaders);
    glUniformMatrix4fv(glGetUniformLocation(colorShaders, "project"), 1, GL_TRUE, project.m);
    glUseProgram(textureShaders);
    glUniformMatrix4fv(glGetUniformLocation(textureShaders, "project"), 1, GL_TRUE, project.m);
}

void display(void)
{
    GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    printError("display pre");
    /* Camera */
    mat4 view = cameraLookAt();
    /* Skybox */
    glUseProgram(textureShaders);
    glDisable(GL_DEPTH_TEST);
    mat4 viewSkyBox = view; // Zero the camera translation but keep the rotation
    viewSkyBox.m[3] = 0;
    viewSkyBox.m[7] = 0;
    viewSkyBox.m[11] = 0;
    mat4 model = IdentityMatrix();
    glUniform1i(glGetUniformLocation(textureShaders, "texUnit"), 0);
    glUniformMatrix4fv(glGetUniformLocation(textureShaders, "view"), 1, GL_TRUE, viewSkyBox.m);
    glUniformMatrix4fv(glGetUniformLocation(textureShaders, "model"), 1, GL_TRUE, model.m);
    DrawModel(skybox, textureShaders, "inVertex", NULL, "inTexCoord");
    glEnable(GL_DEPTH_TEST);
    /* Ground */
    glUniformMatrix4fv(glGetUniformLocation(textureShaders, "view"), 1, GL_TRUE, view.m);
    model = S(100, 0.01, 100);
    glUniform1i(glGetUniformLocation(textureShaders, "texUnit"), 1);
    glUniformMatrix4fv(glGetUniformLocation(textureShaders, "model"), 1, GL_TRUE, model.m);
    DrawModel(cube, textureShaders, "inVertex", NULL, "inTexCoord");
    /* Car */
    float radius = 7;
    float speed = 0.0015;
    float c = cos(t * speed);
    float s = sin(t * speed);
    model = mult3(S(4, 4, 4), T(-radius * c, 0, -radius * s), Ry(-speed * t));
    glUniform1i(glGetUniformLocation(textureShaders, "texUnit"), 2);
    glUniformMatrix4fv(glGetUniformLocation(textureShaders, "model"), 1, GL_TRUE, model.m);
    DrawModel(car, textureShaders, "inVertex", NULL, "inTexCoord");
    /* Windmill */
    glUseProgram(colorShaders);
    glUniformMatrix4fv(glGetUniformLocation(colorShaders, "view"), 1, GL_TRUE, view.m);
    model = Ry(M_PI / 10000 * t);
    // Windmill base
    glUniformMatrix4fv(glGetUniformLocation(colorShaders, "model"), 1, GL_TRUE, model.m);
    DrawModel(wm.walls, colorShaders, "inVertex", "inNormal", NULL);
    DrawModel(wm.roof, colorShaders, "inVertex", "inNormal", NULL);
    DrawModel(wm.balcony, colorShaders, "inVertex", "inNormal", NULL);
    // Windmill blades
    for (int i = 0; i < 4; ++i)
    {
        mat4 tr = T(4.5f, 9.0f, 0.0f); // Position blades relative to model base
        mat4 rx = Rx(
            i * M_PI / 2 +   // Position each blade relative to each other
            -M_PI / 2000 * t // Rotate blades with angular velocity
        );
        mat4 model_i = mult3(model, tr, rx);
        glUniformMatrix4fv(glGetUniformLocation(colorShaders, "model"), 1, GL_TRUE, model_i.m);
        DrawModel(wm.blades, colorShaders, "inVertex", "inNormal", NULL);
    }
    /* Cubes */
    c = cos(t * 0.01);
    s = sin(t * 0.01);
    model = mult4(T(0, 20 + s, -5), Ry(M_PI / 500 * t), Rx(M_PI / 4), Rz(M_PI / 4));
    glUniformMatrix4fv(glGetUniformLocation(colorShaders, "model"), 1, GL_TRUE, model.m);
    DrawModel(cube, colorShaders, "inVertex", "inNormal", NULL);
    model = mult4(T(0, 20 + s, 5), Ry(M_PI / 500 * t), Rx(M_PI / 4), Rz(M_PI / 4));
    glUniformMatrix4fv(glGetUniformLocation(colorShaders, "model"), 1, GL_TRUE, model.m);
    DrawModel(cube, colorShaders, "inVertex", "inNormal", NULL);
    model = mult4(T(5, 20 + s, 0), Ry(M_PI / 500 * t), Rx(M_PI / 4), Rz(M_PI / 4));
    glUniformMatrix4fv(glGetUniformLocation(colorShaders, "model"), 1, GL_TRUE, model.m);
    DrawModel(cube, colorShaders, "inVertex", "inNormal", NULL);
    model = mult4(T(-5, 20 + s, 0), Ry(M_PI / 500 * t), Rx(M_PI / 4), Rz(M_PI / 4));
    glUniformMatrix4fv(glGetUniformLocation(colorShaders, "model"), 1, GL_TRUE, model.m);
    DrawModel(cube, colorShaders, "inVertex", "inNormal", NULL);
    model = mult2(mult4(T(0, 30 - 5 * c, 0), Ry(M_PI / 500 * t), Rx(M_PI / 4), Rz(M_PI / 4)), S(10, 10, 10));
    glUniformMatrix4fv(glGetUniformLocation(colorShaders, "model"), 1, GL_TRUE, model.m);
    DrawModel(cube, colorShaders, "inVertex", "inNormal", NULL);
    printError("draw models");
    //glFinish();
    glutSwapBuffers();
    printError("display after");
}

void onTimer(int value)
{
    mouseWarp(100, 100);
    keyboardHandler();
    glutPostRedisplay();
    glutTimerFunc(20, &onTimer, value);
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitContextVersion(3, 2);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Lab 3.3");
    printError("init glut inits");
    init();
    glutTimerFunc(20, &onTimer, 0);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}