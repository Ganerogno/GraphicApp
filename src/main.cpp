#include <stdlib.h>
#include <stdio.h>

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include "graphic/graphic_container.hpp"

const std::string Shader::PATH = "src/graphic/shaders/";


glm::mat4 transform(glm::vec2 const& Orientation, glm::vec3 const& Translate, glm::vec3 const& Up)
{
    glm::mat4 Proj = glm::perspective(glm::radians(45.f), 1.33f, 0.1f, 10.f);
    glm::mat4 ViewTranslate = glm::translate(glm::mat4(1.f), Translate);
    glm::mat4 ViewRotateX = glm::rotate(ViewTranslate, Orientation.y, Up);
    glm::mat4 View = glm::rotate(ViewRotateX, Orientation.x, Up);
    glm::mat4 Model = glm::mat4(1.0f);
    return Proj * View * Model;
}



int main(void) {
    
    GraphicContainer container;



    container.start();

    glfwTerminate();

    return 0;
}