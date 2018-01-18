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
	program = loadShaders("2/lab2-4.vert", "2/lab2-4.frag");
	printError("init shader");

	// Print setup information
	dumpInfo();

	// Allocate and activate VAO
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);

	// VBO references
	unsigned int vertexBufferObjID;
	unsigned int indexBufferObjID;
	// unsigned int normalBufferObjID;
	unsigned int texCoordBufferObjID;

	// Allocate VBOs
	glGenBuffers(1, &vertexBufferObjID);
	// glGenBuffers(1, &normalBufferObjID);
	glGenBuffers(1, &texCoordBufferObjID);
	glGenBuffers(1, &indexBufferObjID);

	// VBO for vertex datab
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, m->numVertices * 3 * sizeof(GLfloat), m->vertexArray, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "inPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "inPosition"));
	printError("init vbo vertex");

	// VBO for texture coordinates data
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, m->numVertices * 2 * sizeof(GLfloat), m->texCoordArray, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "inTexCoord"), 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "inTexCoord"));
	printError("init vbo tex coord");

	// VBO for index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObjID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->numIndices * sizeof(GLuint), m->indexArray, GL_STATIC_DRAW);
	printError("init vbo index");

	// Load texture
	unsigned int textureID;
	LoadTGATextureSimple("textures/rutor.tga", &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glUniform1i(glGetUniformLocation(program, "texUnit"), 0);
	printError("init texture");

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
	// float a_vel = 2 * M_PI / 10000;
	// GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	
	// Projection
	vec3 camera_pos = {1.0f, 0.0f, 0.0f};
	vec3 camera_look = {0.0f, 0.0f, 0.0f};
	vec3 camera_up = {0.0f, -1.0f, -1.0};
	mat4 project = lookAtv(camera_pos, camera_look, camera_up);

	// Apply rotations/translations
	mat4 transform = T(0.0f, 0.0f, 0.0f);

	// Upload
	//glUniform1f(glGetUniformLocation(program, "inTimeStamp"), t);
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
	glutCreateWindow("Lab 2.4");
	glutTimerFunc(20, &onTimer, 0);
	glutDisplayFunc(display);
	init();
	glutMainLoop();
	return 0;
}
