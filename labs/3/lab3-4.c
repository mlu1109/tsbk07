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
#include "shaders.h"

GLuint pPhongColor;
GLuint pTexture;
GLuint pPhongTexture;

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

void initShaders(void)
{
	pPhongColor = loadShaders("3/phong.vert", "3/phong.frag");
	pTexture = loadShaders("3/texture.vert", "3/texture.frag");
	pPhongTexture = loadShaders("3/phong-texture.vert", "3/phong-texture.frag");

	/* Light */
	GLint isDirectional[] = {false, false, true, true};
	GLfloat lightPosition[] = {
		10.0f, 5.0f, 0.0f, // 1
		0.0f, 5.0f, 10.0f, // 2
		-1.0f, 0.0f, 0.0f, // 3
		0.0f, 0.0f, -1.0f  // 4
	};
	GLfloat lightColor[] = {
		1.0f, 0.0f, 0.0f, // 1: Red
		0.0f, 1.0f, 0.0f, // 2: Green
		0.0f, 0.0f, 1.0f, // 3: Blue
		1.0f, 1.0f, 1.0f  // 4: White
	};
	// pPhongTexture
	glUseProgram(pPhongTexture);
	glUniform3fv(glGetUniformLocation(pPhongTexture, "lightPosition"), 4, lightPosition);
	glUniform3fv(glGetUniformLocation(pPhongTexture, "lightColor"), 4, lightColor);
	glUniform1iv(glGetUniformLocation(pPhongTexture, "isDirectional"), 4, isDirectional);
	// pPhongColor
	glUseProgram(pPhongColor);
	glUniform3fv(glGetUniformLocation(pPhongColor, "lightPosition"), 4, lightPosition);
	glUniform3fv(glGetUniformLocation(pPhongColor, "lightColor"), 4, lightColor);
	glUniform1iv(glGetUniformLocation(pPhongColor, "isDirectional"), 4, isDirectional);

	printError("init shader");
}

void initModels(void)
{
	/* Models */
	wm.blades = LoadModelPlus("models/windmill/blade.obj");
	wm.walls = LoadModelPlus("models/windmill/windmill-walls.obj");
	wm.balcony = LoadModelPlus("models/windmill/windmill-balcony.obj");
	wm.roof = LoadModelPlus("models/windmill/windmill-roof.obj");
	skybox = LoadModelPlus("models/skybox.obj");
	cube = LoadModelPlus("models/cubeplus.obj");
	car = LoadModelPlus("models/bilskiss.obj");
	/* Init textures */
	GLuint skyboxTexture;
	GLuint carTexture;
	GLuint groundTexture;
	// Skybox
	glActiveTexture(GL_TEXTURE0);
	LoadTGATextureSimple("textures/SkyBox512.tga", &skyboxTexture);
	glBindTexture(GL_TEXTURE_2D, skyboxTexture);
	// Ground
	glActiveTexture(GL_TEXTURE1);
	LoadTGATextureSimple("textures/grass.tga", &groundTexture);
	glBindTexture(GL_TEXTURE_2D, groundTexture);
	// Car
	glActiveTexture(GL_TEXTURE2);
	LoadTGATextureSimple("models/bilskissred.tga", &carTexture);
	glBindTexture(GL_TEXTURE_2D, carTexture);
	printError("init model");
}

void init(void)
{
	dumpInfo();
	/* Init gl */
	glClearColor(0.2, 0.2, 0.5, 0);
	initShaders();
	initModels();
	inputInit();
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
}

void display(void)
{
	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	printError("display pre");
	/* Camera */
	mat4 view = cameraLookAt();
	/* Skybox */
	glUseProgram(pTexture);
	glDisable(GL_DEPTH_TEST);
	// Zero the camera translation but keep the rotation
	mat4 viewSkyBox = view;
	viewSkyBox.m[3] = 0;
	viewSkyBox.m[7] = 0;
	viewSkyBox.m[11] = 0;
	mat4 mvp = mult2(project, viewSkyBox);
	shaderUpload(pTexture, &mvp, NULL, 0, -1);
	DrawModel(skybox, pTexture, "inVertex", NULL, "inTexCoord");
	glEnable(GL_DEPTH_TEST);
	/* Ground */
	mat4 transform = S(100, 0.01, 100);
	mvp = mult3(project, view, transform);
	shaderUpload(pTexture, &mvp, NULL, 1, -1);
	DrawModel(cube, pTexture, "inVertex", NULL, "inTexCoord");
	/* -------------------- *
	 * Phong texture shader *
	 * -------------------- */
	glUseProgram(pPhongTexture);
	glUniform3fv(glGetUniformLocation(pPhongTexture, "cameraPosition"), 1, &camera.pos.x);
	/* Car */
	float radius = 20;
	float speed = 0.0015;
	float c = cos(t * speed);
	float s = sin(t * speed);
	transform = mult3(
		T(-radius * c, 0, -radius * s),
		Ry(-speed * t),
		S(4, 4, 4));
	mvp = mult3(project, view, transform);
	shaderUpload(pPhongTexture, &mvp, &transform, 2, 250);
	DrawModel(car, pPhongTexture, "inVertex", "inNormal", "inTexCoord");
	/* ------------------- *
	 * Phong color shaders *
	 * ------------------- */
	glUseProgram(pPhongColor);
	glUniform3fv(glGetUniformLocation(pPhongColor, "cameraPosition"), 1, &camera.pos.x);
	/* Windmill */
	transform = Ry(M_PI / 50000 * t);
	// Windmill base
	mvp = mult3(project, view, transform);
	shaderUpload(pPhongColor, &mvp, &transform, -1, 50);
	DrawModel(wm.walls, pPhongColor, "inVertex", "inNormal", NULL);
	DrawModel(wm.roof, pPhongColor, "inVertex", "inNormal", NULL);
	DrawModel(wm.balcony, pPhongColor, "inVertex", "inNormal", NULL);
	// Windmill blades
	for (int i = 0; i < 4; ++i)
	{
		mat4 transform_i = mult3(
			transform,
			T(4.5f, 9.0f, 0.0f),
			Rx(i * M_PI / 2 + -M_PI / 2000 * t));
		mvp = mult3(project, view, transform_i);
		shaderUpload(pPhongColor, &mvp, &transform_i, -1, -1);
		DrawModel(wm.blades, pPhongColor, "inVertex", "inNormal", NULL);
	}
	/* Cubes */
	c = cos(t * 0.001);
	s = sin(t * 0.001);
	transform = mult3(
		Ry(M_PI / 3000 * t),
		Rx(M_PI / 4),
		Rz(M_PI / 4));
	mat4 cubeTranslation[4] = {
		T(-5, 15 + s, 0),
		T(0, 15 + s, -5),
		T(0, 15 + s, 5),
		T(5, 15 + s, 0)};
	for (int i = 0; i < 4; ++i)
	{
		mat4 transform_i = mult2(cubeTranslation[i], transform);
		mvp = mult3(project, view, transform_i);
		shaderUpload(pPhongColor, &mvp, &transform_i, -1, 30);
		DrawModel(cube, pPhongColor, "inVertex", "inNormal", NULL);
	}
	transform = mult3(T(0, 30 - 5 * c, 0), S(10, 10, 10), transform);
	mvp = mult3(project, view, transform);
	shaderUpload(pPhongColor, &mvp, &transform, -1, 10);
	DrawModel(cube, pPhongColor, "inVertex", "inNormal", NULL);
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
	glutCreateWindow("Lab 3.4");
	printError("init glut inits");
	init();
	glutTimerFunc(20, &onTimer, 0);
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}