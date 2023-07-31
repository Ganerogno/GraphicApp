#pragma once
#include "render_interface.hpp"
#include "state_interface.hpp"

class GraphicObject : public RenderInterface, public StateInterface
{
protected:
    glm::vec3 position;
public:
    GraphicObject(Shader& shader);
    virtual bool init() = 0;
    virtual void draw(glm::mat4 proj, glm::mat4 view) = 0;
    virtual void update() = 0;
};