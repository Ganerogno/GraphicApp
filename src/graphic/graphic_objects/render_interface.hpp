#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include <glm/ext.hpp> // perspective, translate, rotate

#include "../functions/shader.hpp"

class RenderInterface
{
protected:
    
    GLuint EBO;
    GLuint VBO;
    GLuint VAO;
    glm::mat4 model;
    Shader& shader;
    GLuint indicies_count;
public:
    RenderInterface(Shader& shader) : shader(shader){}
    bool virtual init() = 0;
    void virtual draw(glm::mat4 proj, glm::mat4 view) = 0;
};