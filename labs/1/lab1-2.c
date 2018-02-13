#ifdef __APPLE__
#include <OpenGL/gl3.h>
// Linking hint for Lightweight IDE
// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"

// Globals
GLfloat vertices[] = {
	-0.5f, -0.5f, 0.0f,
	-0.5f, 0.5f, 0.0f,
	0.5f, -0.5f, 0.0f};

GLfloat myMatrix[] = {1.0f, 0.0f, 0.0f, 0.5f,
					  0.0f, 1.0f, 0.0f, 0.0f,
					  0.0f, 0.0f, 1.0f, 0.0f,
					  0.0f, 0.0f, 0.0f, 1.0f};

// VAO reference
unsigned int vertexArrayObjID;

void init(void)
{
	// vertex buffer object, used for uploading the geometry
	unsigned int vertexBufferObjID;
	// Reference to shader program
	GLuint program;

	dumpInfo();

	// GL inits
	glClearColor(0.2, 0.2, 0.5, 0);
	glDisable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("1/lab1-2.vert", "1/lab1-2.frag");
	printError("init shader");

	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);
	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &vertexBufferObjID);

	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "inPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "inPosition"));

	glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, myMatrix);
	// End of upload of geometry

	printError("init arrays");
}

void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(vertexArrayObjID); // Select VAO
	glDrawArrays(GL_TRIANGLES, 0, 3);	// draw object

	printError("display");

	glFinish();
	//glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutCreateWindow("Lab 1.2");
	glutDisplayFunc(display);
	init();
	glutMainLoop();
	return 0;
}
