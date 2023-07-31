#include "graphic_object.hpp"

class TestObject : public GraphicObject
{
    public:


    GLfloat vertices[12] = {
         0.5f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };
    GLuint indices[6] = {
        0, 1, 3,
        1, 2, 3
    };
    
    GLfloat color[4] = {1.0f, 0.0f, 0.0f, 1.0f};

    TestObject(Shader& shader) : GraphicObject(shader)
    {

    }
        
    bool init() override;
    void draw(glm::mat4 proj, glm::mat4 view) override;
    void update() override;
};