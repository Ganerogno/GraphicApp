#include <GLFW/glfw3.h>
#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include <glm/ext.hpp> // perspective, translate, rotate

#include "graphic/graphic_objects/graphic_object.hpp"
#include "graphic/graphic_objects/slow_state_interface.hpp"
#include "functions/shader.hpp"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

class GraphicContainer
{
    class Settings
    {
        int width;
        int height;
        public:
        Settings()
        {
            width = 800;
            height = 600;
        }
        int get_width() const
        {
            return width;
        }
        int get_height() const
        {
            return height;
        }
    };
    class Camera
    {
        glm::vec3 cameraPos;
	    static glm::vec3 cameraDir;
	    glm::vec3 cameraUp;
	    GLfloat cameraSpeed = 0.05f;
	    static glm::mat4x4 ViewMatrix;
    };
    class World : public GraphicObject, public SlowStateInterface
    {

    };
    class Save
    {

    };
    
    Settings settings;
    GLFWwindow* window;
    glm::mat4 proj;
    
    public:
    GraphicContainer()
    {
        glfwInit();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(settings.get_width(), settings.get_height(), "GraphicApp", NULL, NULL);
        glfwMakeContextCurrent(window);

        glfwSetKeyCallback(window, key_callback);
        gladLoadGL();
    }
    void start()
    {
        proj = glm::perspective( 45.0f, (GLfloat)settings.get_width()/(GLfloat)settings.get_height(), 0.1f, 100.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
        glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        Shader testShader("test",true,true);
        GLfloat vertices[] = {
     0.5f,  0.5f, 0.0f,  // Верхний правый угол
     0.5f, -0.5f, 0.0f,  // Нижний правый угол
    -0.5f, -0.5f, 0.0f,  // Нижний левый угол
    -0.5f,  0.5f, 0.0f   // Верхний левый угол
        };
        GLuint indices[] = {  // Помните, что мы начинаем с 0!
            0, 1, 3,   // Первый треугольник
            1, 2, 3    // Второй треугольник
        };  
        GLuint EBO;
        glGenBuffers(1, &EBO);

        GLuint VBO;
        glGenBuffers(1, &VBO);
        GLuint VAO;
        glGenVertexArrays(1, &VAO);



        glBindVertexArray(VAO);
        // 2. Копируем наш массив вершин в буфер для OpenGL
        glBindBuffer(GL_ARRAY_BUFFER, VBO); 
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        // 3. Устанавливаем указатели на вершинные атрибуты 
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0); 
        
            GLint modelLoc = glGetUniformLocation(testShader.GetProgram(), "model");
            GLint projLoc = glGetUniformLocation(testShader.GetProgram(), "projection");
            GLint viewLoc = glGetUniformLocation(testShader.GetProgram(), "view");

        glClearColor(0.2f, 0.1f, 0.5f, 1.0f);
        while (!glfwWindowShouldClose(window)) 
        {
            glfwPollEvents();
            glClear(GL_COLOR_BUFFER_BIT);
            testShader.Use();
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
            testShader.UseBaseShader();


            glfwSwapBuffers(window);
        }
    }
};