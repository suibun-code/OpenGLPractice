#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

#include "ShaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "IBO.h"

GLfloat vertices[] =
{
//		  POSITION	   ||      COLOR
	-0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f, //Lower left corner.
	-0.5f,  0.5f, 0.0f,   0.0f, 1.0f, 0.0f, //Lower right corner.
	 0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f, //Upper corner.
	 0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f, //Inner left corner.
	
 //  -0.5f, -0.5f * float(sqrt(3)) / 3,     0.0f,   0.8f, 0.3f, 0.02f, //Lower left corner.
	//0.5f, -0.5f * float(sqrt(3)) / 3,     0.0f,   0.8f, 0.3f, 0.02f, //Lower right corner.
	//0.0f,  0.5f * float(sqrt(3)) * 2 / 3, 0.0f,   1.0f, 0.6f, 0.32f, //Upper corner.

 //  -0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,    0.9f, 0.3f, 0.17f, //Inner left corner.
	//0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,    0.9f, 0.3f, 0.17f, //Inner right corner.
	//0.0f,    -0.5f * float(sqrt(3)) / 3, 0.0f,    0.8f, 0.3f, 0.02f  //Inner down corner.

};

GLuint indices[] =
{
	0, 2, 1, //Upper triangle.
	0, 3, 2  //Lower triangle.
	
	//0, 3, 5, //Lower left triangle.
	//3, 2, 4, //Lower right triangle.
	//5, 4, 1 //Upper triangle.
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
	
	vao1.LinkAttrib(vbo1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	vao1.LinkAttrib(vbo1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	vao1.Unbind();
	vbo1.Unbind();
	ibo1.Unbind();

	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");
	

	//Clear to a new color and swap buffers.
	glClearColor(0.07, 0.13f, 0.17f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);

	//Loop to poll events.
	while(!glfwWindowShouldClose(window))
	{
		glClearColor(0.07, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shaderProgram.Activate();
		glUniform1f(uniID, 1.5f);
		vao1.Bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		
		glfwPollEvents();
	}

	vao1.Delete();
	vbo1.Delete();
	ibo1.Delete();
	shaderProgram.Delete();

	//Cleanup and final return.
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}