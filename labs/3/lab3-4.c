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

const float k_a = 0.1;
const float k_s = 1.0;
const float k_d = 0.6;

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
Model *bunny;

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
	/* Init projection */
	project = frustum(
		-0.5f, // Left
		0.5f,  // Right
		-0.5f, // Bottom
		0.5f,  // Top
		1.0f,  // Near
		300.0f // Far
	);
	// pTexture
	glUseProgram(pTexture);
	glUniformMatrix4fv(glGetUniformLocation(pTexture, "project"), 1, GL_TRUE, project.m);

	// pPhongTexture
	glUseProgram(pPhongTexture);
	glUniform3fv(glGetUniformLocation(pPhongTexture, "lightPosition"), 4, lightPosition);
	glUniform3fv(glGetUniformLocation(pPhongTexture, "lightColor"), 4, lightColor);
	glUniform1iv(glGetUniformLocation(pPhongTexture, "isDirectional"), 4, isDirectional);
	glUniformMatrix4fv(glGetUniformLocation(pPhongTexture, "project"), 1, GL_TRUE, project.m);
	glUniform1f(glGetUniformLocation(pPhongTexture, "k_a"), k_a);
	glUniform1f(glGetUniformLocation(pPhongTexture, "k_d"), k_d);
	glUniform1f(glGetUniformLocation(pPhongTexture, "k_s"), k_s);

	// pPhongColor
	glUseProgram(pPhongColor);
	glUniform3fv(glGetUniformLocation(pPhongColor, "lightPosition"), 4, lightPosition);
	glUniform3fv(glGetUniformLocation(pPhongColor, "lightColor"), 4, lightColor);
	glUniform1iv(glGetUniformLocation(pPhongColor, "isDirectional"), 4, isDirectional);
	glUniformMatrix4fv(glGetUniformLocation(pPhongColor, "project"), 1, GL_TRUE, project.m);
	glUniform1f(glGetUniformLocation(pPhongColor, "k_a"), k_a);
	glUniform1f(glGetUniformLocation(pPhongColor, "k_d"), k_d);
	glUniform1f(glGetUniformLocation(pPhongColor, "k_s"), k_s);

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
	bunny = LoadModelPlus("models/bunnyplus.obj");
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
	mat4 model = IdentityMatrix();
	shaderUpload(pTexture, &model, &viewSkyBox, 0, -1);
	DrawModel(skybox, pTexture, "inVertex", NULL, "inTexCoord");
	glEnable(GL_DEPTH_TEST);
	/* -------------------- *
	 * Phong texture shader *
	 * -------------------- */
	glUseProgram(pPhongTexture);
	glUniform3fv(glGetUniformLocation(pPhongTexture, "cameraPosition"), 1, &camera.pos.x);
	/* Ground */
	model = S(100, 0.01, 100);
	shaderUpload(pPhongTexture, &model, &view, 1, 2500);
	DrawModel(cube, pPhongTexture, "inVertex", "inNormal", "inTexCoord");
	/* Car */
	float radius = 20;
	float speed = 0.0015;
	float c = cos(t * speed);
	float s = sin(t * speed);
	model = mult3(
		T(-radius * c, 0, -radius * s),
		Ry(-speed * t),
		S(4, 4, 4));
	shaderUpload(pPhongTexture, &model, NULL, 2, 250);
	DrawModel(car, pPhongTexture, "inVertex", "inNormal", "inTexCoord");
	/* ------------------- *
	 * Phong color shaders *
	 * ------------------- */
	glUseProgram(pPhongColor);
	glUniform3fv(glGetUniformLocation(pPhongColor, "cameraPosition"), 1, &camera.pos.x);
	/* Windmill */
	model = Ry(M_PI / 50000 * t);
	// Windmill base
	shaderUpload(pPhongColor, &model, &view, -1, 50);
	DrawModel(wm.walls, pPhongColor, "inVertex", "inNormal", NULL);
	DrawModel(wm.roof, pPhongColor, "inVertex", "inNormal", NULL);
	DrawModel(wm.balcony, pPhongColor, "inVertex", "inNormal", NULL);
	// Windmill blades
	for (int i = 0; i < 4; ++i)
	{
		mat4 model_i = mult3(
			model,
			T(4.5f, 9.0f, 0.0f),
			Rx(i * M_PI / 2 + -M_PI / 2000 * t));
		shaderUpload(pPhongColor, &model_i, NULL, -1, -1);
		DrawModel(wm.blades, pPhongColor, "inVertex", "inNormal", NULL);
	}
	/* Cubes */
	c = cos(t * 0.001);
	s = sin(t * 0.001);
	model = mult3(
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
		mat4 model_i = mult2(cubeTranslation[i], model);
		shaderUpload(pPhongColor, &model_i, NULL, -1, 30);
		DrawModel(cube, pPhongColor, "inVertex", "inNormal", NULL);
	}
	model = mult3(T(0, 30 - 5 * c, 0), S(10, 10, 10), model);
	shaderUpload(pPhongColor, &model, NULL, -1, 10);
	DrawModel(cube, pPhongColor, "inVertex", "inNormal", NULL);
	/* Bunnies */
	model = S(5, 5, 5);
	mat4 bunnyTranslation[4] = {
		T(30, 3, 0),
		T(-30, 3, 0),
		T(0, 3, -30),
		T(0, 3, 30)};
	for (int i = 0; i < 4; ++i)
	{
		mat4 model_i = mult2(bunnyTranslation[i], model);
		shaderUpload(pPhongColor, &model_i, NULL, -1, 125);
		DrawModel(bunny, pPhongColor, "inVertex", "inNormal", NULL);
	}
	/* Done */
	printError("draw models");
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