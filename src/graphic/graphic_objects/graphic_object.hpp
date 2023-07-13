#include "render_interface.hpp"
#include "state_interface.hpp"
#include <glm/glm.hpp>

class GraphicObject : public RenderInterface, public StateInterface
{
    glm::vec3 position;
    glm::vec3 view_direction;
public:
    bool init();
    void draw();
    void update();
};