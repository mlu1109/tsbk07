#ifdef __APPLE__
#include <OpenGL/gl3.h>
#endif
// Common
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "loadobj.h"
#include "VectorUtils3.h"
// Own
#include "algebra.h"
#include "camera.h"
#include "input.h"

GLuint program;
mat4 project;

struct WindMill
{
	Model *blades;
	Model *walls;
	Model *balcony;
	Model *roof;
} wm;

void init(void)
{
	dumpInfo();

	program = loadShaders("3/normal.vert", "3/normal.frag");
	printError("init shader");

	glClearColor(0.2, 0.2, 0.5, 0);
	glEnable(GL_DEPTH_TEST);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	printError("init gl inits");
	// Init input
	inputInit();
	glutMouseFunc(&mouseButtonFunc);
	glutPassiveMotionFunc(&mouseMotionFunc);
	// Init model
	wm.blades = LoadModelPlus("models/windmill/blade.obj");
	wm.walls = LoadModelPlus("models/windmill/windmill-walls.obj");
	wm.balcony = LoadModelPlus("models/windmill/windmill-balcony.obj");
	wm.roof = LoadModelPlus("models/windmill/windmill-roof.obj");
	// Init camera
	vec3 camera_pos = {0.0f, 0.0f, -30.0f};
	vec3 camera_look = {0.0f, 0.0f, 0.0f};
	vec3 camera_up = {0.0f, 1.0f, 0.0f};
	cameraInit(camera_pos, camera_look, camera_up);
	// Init projection
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
	printError("display pre");
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Camera
	mat4 view = cameraLookAt();
	// Windmill
	mat4 transform = mult2(
		Ry(M_PI / 10000 * t),
		T(0, -7, 0) // Center the model
	);
	// Windmill base
	mat4 mvp = mult3(project, view, transform);
	glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_TRUE, mvp.m);
	DrawModel(wm.walls, program, "inVertex", "inNormal", NULL);
	DrawModel(wm.roof, program, "inVertex", "inNormal", NULL);
	DrawModel(wm.balcony, program, "inVertex", "inNormal", NULL);
	// Windmill blades
	for (int i = 0; i < 4; ++i)
	{
		mat4 tr = T(4.5f, 9.0f, 0.0f); // Position blades relative to model base
		mat4 rx = Rx(
			i * M_PI / 2 +   // Position each blade
			-M_PI / 2000 * t // Angular velocity * time
		);
		mat4 transform_i = mult3(transform, tr, rx);
		mvp = mult3(project, view, transform_i);
		glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_TRUE, mvp.m);
		DrawModel(wm.blades, program, "inVertex", "inNormal", NULL);
	}
	glFinish();
	//glutSwapBuffers();
	printError("display after");
}

void onTimer(int value)
{
	keyboardHandler();
	glutPostRedisplay();
	glutTimerFunc(20, &onTimer, value);
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutCreateWindow("Lab 3.2");
	glutTimerFunc(20, &onTimer, 0);
	glutDisplayFunc(display);
	init();
	glutMainLoop();
	return 0;
}