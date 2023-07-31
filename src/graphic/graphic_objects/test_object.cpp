#include "test_object.hpp"

bool TestObject::init()
{
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    model = glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    std::cout<<"Object: "<<shader.GetProgram()<<std::endl;
    return true;
}
void TestObject::draw(glm::mat4 proj, glm::mat4 view)
{
    GLint modelLoc = glGetUniformLocation(shader.GetProgram(), "model");
    GLint projLoc = glGetUniformLocation(shader.GetProgram(), "projection");
    GLint viewLoc = glGetUniformLocation(shader.GetProgram(), "view");
    GLint colorLoc = glGetUniformLocation(shader.GetProgram(), "in_color");
    shader.Use();
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniform4fv(colorLoc, 1, color);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO); 
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    shader.UseBaseShader();
}
void TestObject::update()
{

}