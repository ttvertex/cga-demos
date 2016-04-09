#include "TessellatedQuad.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

using namespace std;

TessellatedQuad::TessellatedQuad(GLFWwindow* window, int size)
{
	this->size = size;
	this->window = window;
	planePos = vec3(0.0f, 0.0f, 2.5f);
}

void TessellatedQuad::init()
{
	genPlane();
	genBuffers();

	// init matrices
	modelMatrix = glm::mat4(1.0f);
	viewMatrix = glm::lookAt(
		vec3(0.0f, 0.0f, -1.0f), //eye
		vec3(0.0f, 0.0f, 0.0f), //center
		vec3(0.0f, 1.0f, 0.0f)); //up
	projectionMatrix = glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 100.0f);

	// load shaders
	try {
		shader.compileShader("shader/glsl40_basic_tess.vert", GLSLShader::VERTEX);
		shader.compileShader("shader/glsl40_basic_tess.frag", GLSLShader::FRAGMENT);
		shader.compileShader("shader/glsl40_basic_tess.tcs", GLSLShader::TESS_CONTROL);
		shader.compileShader("shader/glsl40_basic_tess.tes", GLSLShader::TESS_EVALUATION);

		shader.link();
		shader.use();
	}
	catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
	shader.printActiveAttribs();
}

void TessellatedQuad::update(double deltaTime)
{
	//// matrices setup
	modelMatrix = mat4(1.0f); // identity
	modelMatrix = glm::translate(modelMatrix, planePos); // translate back
	modelViewMatrix = viewMatrix * modelMatrix;
	modelViewProjectionMatrix = projectionMatrix * modelViewMatrix;

	// set var MVP on the shader
	shader.setUniform("MVP", modelViewProjectionMatrix); //ModelViewProjection
}

void TessellatedQuad::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(vaoID);
	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glDrawElements(GL_PATCHES, indices.size(), GL_UNSIGNED_INT, (GLubyte *)NULL);
	glBindVertexArray(0);
}

void TessellatedQuad::genBuffers()
{
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	unsigned int handle[3];
	glGenBuffers(3, handle);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), (GLvoid*)&vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
	glEnableVertexAttribArray(0);  // Vertex position -> layout 0 in the VS

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(vec3), (GLvoid*)&colors[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
	glEnableVertexAttribArray(1);  // Vertex color -> layout 1 in the VS

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), (GLvoid*)&indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void TessellatedQuad::resize(int x, int y)
{

}

void TessellatedQuad::genPlane()
{
	// v0 -- bottom left
	vertices.push_back(vec3(-size, -size, 0.0f));
	colors.push_back(vec3(1.0f, 0.0f, 0.0f));

	//v1 -- bottom right
	vertices.push_back(vec3(size, -size, 0.0f));
	colors.push_back(vec3(0.0f, 1.0f, 0.0f));

	//v2 -- top left
	vertices.push_back(vec3(-size, size, 0.0f));
	colors.push_back(vec3(0.0f, 0.0f, 1.0f));

	////v3 -- top right
	vertices.push_back(vec3(size, size, 0.0f));
	colors.push_back(vec3(1.0f, 1.0f, 0.0f));
	
	// we'll have two triangles, one being v0,v1,v2 and the other v2,v3,v0
	// triangle 1
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);

	//// triangle 2
	//indices.push_back(2);
	//indices.push_back(1);
	//indices.push_back(3);
}
