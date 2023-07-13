#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

class Shader
{
    const static std::string PATH;

	std::string shaderString;
	GLuint program;
	void AddShader(std::string path, GLenum flag)
	{
		std::stringstream shaderStream;
		std::ifstream shaderFile;
		shaderFile.open(path);
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
		shaderString = shaderStream.str();

		GLuint shader;
		GLint succes;
		GLchar log[512];

		const GLchar* str = shaderString.c_str();
		shader = glCreateShader(flag);
		glShaderSource(shader, 1, &str, NULL);
		glCompileShader(shader);

		glGetShaderiv(shader, GL_COMPILE_STATUS, &succes);
		if (!succes)
		{
			glGetShaderInfoLog(shader, 512, NULL, log);
			std::cout << "Compile shader error path: " << path << " error:" << log << std::endl;
		}
		glAttachShader(program, shader);
		glDeleteShader(shader);
	}
	void CreateProgram()
	{
		GLint succes;
		GLchar log[512];
		glLinkProgram(program);
		glGetShaderiv(program, GL_LINK_STATUS, &succes);
		if (!succes)
		{
			std::cout << "Link program error: " << log << std::endl;
		}
	}
public:
	Shader(const char* name, bool isVert, bool isFrag)
	{
		program = glCreateProgram();
		if (isVert)
			AddShader(PATH + name + ".vert", GL_VERTEX_SHADER);
		if (isFrag)
			AddShader(PATH + name + ".frag", GL_FRAGMENT_SHADER);

		CreateProgram();
	}
	
	void Use()
	{
		glUseProgram(program);
	}
	void UseBaseShader()
	{
		glUseProgram(0);
	}
	GLuint GetProgram()
	{
		return program;
	}
};