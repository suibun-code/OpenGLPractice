#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

#include "ShaderClass.h"
#include "Texture.h"
#include "Camera.h"

#include "VAO.h"
#include "VBO.h"
#include "IBO.h"

GLfloat vertices[] =
{
//	      POSITION	    ||       COLOR        ||   TEXTURE
	-0.5f, 0.0f,  0.5f,   0.83f, 0.70f, 0.44f,   0.0f, 0.0f,
	-0.5f, 0.0f, -0.5f,   0.83f, 0.70f, 0.44f,   5.0f, 0.0f,
	 0.5f, 0.0f, -0.5f,   0.83f, 0.70f, 0.44f,   0.0f, 0.0f,
	 0.5f, 0.0f,  0.5f,   0.83f, 0.70f, 0.44f,   5.0f, 0.0f,
	 0.0f, 0.8f,  0.0f,   0.92f, 0.86f, 0.76f,   2.5f, 5.0f

};

GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4
};

int main()
{
	glfwInit();

	//GLFW hints.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 8);

	//Create and use window, as well as do error checking to make sure it is created properly.
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Practice", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window.\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//Load Glad.
	gladLoadGL();

	//Set OpenGL viewport.
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	Shader shaderProgram("default.vert", "default.frag");

	VAO vao1;
	vao1.Bind();

	VBO vbo1(vertices, sizeof(vertices));
	IBO ibo1(indices, sizeof(indices));

	vao1.LinkAttrib(vbo1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	vao1.LinkAttrib(vbo1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	vao1.LinkAttrib(vbo1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	vao1.Unbind();
	vbo1.Unbind();
	ibo1.Unbind();

	//Texture
	Texture shrek("brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	shrek.TexUnit(shaderProgram, "tex0", 0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

	Camera camera(WINDOW_WIDTH, WINDOW_HEIGHT, glm::vec3(0.f, 0.f, 2.f));

	//Loop to poll events.
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderProgram.Activate();

		camera.Inputs(window);
		camera.Matrix(45.f, 0.1f, 100.f, shaderProgram, "camMatrix");
		
		shrek.Bind();
		vao1.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	vao1.Delete();
	vbo1.Delete();
	ibo1.Delete();
	shrek.Delete();
	shaderProgram.Delete();

	//Cleanup and final return.
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}