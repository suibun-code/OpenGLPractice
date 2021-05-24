#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

std::string GetFileContents(const char* filename);

class Shader
{
private:
	void CompileErrors(unsigned int shader, const char* type);

public:
	GLuint ID;
	Shader(const char* vertexFile, const char* fragmentFile);

	void Activate();
	void Delete();
};

