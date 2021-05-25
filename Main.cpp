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

	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	//Texture
	Texture shrek("brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	shrek.TexUnit(shaderProgram, "tex0", 0);

	float rotation = 0.f;
	double prevTime = glfwGetTime();

	glEnable(GL_DEPTH_TEST);

	//Loop to poll events.
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderProgram.Activate();

		double crntTime = glfwGetTime();
		if (crntTime - prevTime >= 1 / 165)
		{
			rotation += 0.2f;
			prevTime = crntTime;
		}

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.f, 1.f, 0.f));
		view = glm::translate(view, glm::vec3(0.f, -0.5f, -2.0f));
		projection = glm::perspective(glm::radians(45.0f), (float)(WINDOW_WIDTH / WINDOW_HEIGHT), 0.1f, 100.f);

		int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		int projectionLoc = glGetUniformLocation(shaderProgram.ID, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glUniform1f(uniID, 1.f);
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