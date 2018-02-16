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

mat4 projectionMatrix;
// Keyboard translation for model
mat4 keyboardTranslation;
// vertex array object
Model *m, *m2, *tm;
Object groundSphere;
Object octagon;
// Reference to shader program
GLuint program;
GLuint pPhong;
GLuint tex1, tex2;
TextureData ttex; // terrain

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

	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 300.0);

	// Textures
	LoadTGATextureSimple("textures/rutor.tga", &tex1);
	// Shader "program"
	program = loadShaders("4/terrain.vert", "4/terrain.frag");
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "project"), 1, GL_TRUE, projectionMatrix.m);
	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
	printError("init shader program");

	// Shader "pPhong"
	pPhong = loadShaders("4/phong.vert", "4/phong.frag");
	glUseProgram(pPhong);
	glUniformMatrix4fv(glGetUniformLocation(pPhong, "project"), 1, GL_TRUE, projectionMatrix.m);
	printError("init shader pPhong");

	// Load terrain
	LoadTGATextureData("textures/fft-terrain.tga", &ttex);
	tm = generateTerrain(&ttex, quadPixelWidth, quadPixelHeight);
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
	mat4 modelView, model, camMatrix;

	printError("pre display");

	camMatrix = cameraLookAt();

	// Terrain
	glUseProgram(program);
	glUniform3fv(glGetUniformLocation(program, "cameraPosition"), 1, &camera.pos.x);
	model = IdentityMatrix();
	modelView = Mult(camMatrix, model);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelView"), 1, GL_TRUE, modelView.m);
	glBindTexture(GL_TEXTURE_2D, tex1);
	DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");
	printError("display 2");
	
	// Ground Sphere
	glUseProgram(pPhong);
	glUniform3fv(glGetUniformLocation(pPhong, "cameraPosition"), 1, &camera.pos.x);
	model = objectGetModelMatrix(&groundSphere);
	modelView = Mult(camMatrix, model);
	glUniformMatrix4fv(glGetUniformLocation(pPhong, "modelView"), 1, GL_TRUE, modelView.m);
	DrawModel(groundSphere.model, program, "inPosition", "inNormal", NULL);
	printError("display 3");
	 
	glutSwapBuffers();
}

void timer(int i)
{
	mouseWarp(100, 100);
	keyboardHandler(&groundSphere);
	groundSphere.position.y = calcY(tm->vertexArray, groundSphere.position.x, groundSphere.position.z, quadPixelWidth, quadPixelHeight, ttex.width, ttex.height);
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
