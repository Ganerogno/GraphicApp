#include <GLFW/glfw3.h>

#include "graphic/graphic_objects/graphic_object.hpp"
#include "graphic/graphic_objects/slow_state_interface.hpp"


class GraphicContainer
{
    class Settings
    {

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
    
    public:
};