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
	void AddShader(std::string path, GLenum flag);
	void CreateProgram();
public:
	Shader();
	Shader(const char* name, bool isVert, bool isFrag);
	
	void Use();
	void UseBaseShader();
	GLuint GetProgram();
};