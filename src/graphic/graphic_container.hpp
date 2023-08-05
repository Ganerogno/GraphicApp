#pragma once
#include <chrono>
#include <thread>
#include <map>
#include <vector>

#include "graphic_objects/graphic_object.hpp"
#include "graphic_objects/test_object.hpp"
#include "graphic_objects/slow_state_interface.hpp"

#define CHUNK_SIZE 16

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

class GraphicContainer
{
    class Settings //Settings/////////////////////////
    {
    public:
        int width;
        int height;
        GLFWwindow* window;

        bool can_fly;

        Settings();
    };
    static Settings settings;
    static Shader default_shader;

    class Camera : public StateInterface //Camera///////////////////////////
    {
        public:
        static glm::vec3 position;
	    static glm::vec3 cameraDir;
	    glm::vec3 cameraUp;
        glm::vec3 cameraRight;
	    GLfloat cameraSpeed = 0.15f;
	    static GLfloat yaw;
	    static GLfloat pitch;
	    glm::mat4x4 view;
        
        Camera() : Camera(glm::vec3(0.0f,2.0f,3.0f), glm::vec3(0.0f,2.0f,0.0f))
        {

        }
        Camera(glm::vec3 pos, glm::vec3 target)
        {
            position = pos;
            cameraDir = glm::normalize(target - pos);
            glm::vec3 up = glm::vec3(0,1,0);
            cameraRight = glm::normalize(glm::cross(up, cameraDir));
            cameraUp = glm::normalize(glm::cross(cameraDir, cameraRight));
            view = glm::lookAt(position, cameraDir + position, cameraUp);
        }
        void update() override
        {
            if(settings.can_fly)
            {
                if(keys[GLFW_KEY_W])
                    position += cameraSpeed * cameraDir;
                if(keys[GLFW_KEY_S])
                    position -= cameraSpeed * cameraDir;
                if(keys[GLFW_KEY_A])
                    position -= glm::normalize(glm::cross(cameraDir, cameraUp)) * cameraSpeed;
                if(keys[GLFW_KEY_D])
                    position += glm::normalize(glm::cross(cameraDir, cameraUp)) * cameraSpeed; 
            }
            else
            {
                if(keys[GLFW_KEY_W])
                    position += cameraSpeed * glm::normalize(glm::vec3(cameraDir.x,0,cameraDir.z));
                if(keys[GLFW_KEY_S])
                    position -= cameraSpeed * glm::normalize(glm::vec3(cameraDir.x,0,cameraDir.z));
                if(keys[GLFW_KEY_A])
                    position -= glm::normalize(glm::cross(glm::normalize(glm::vec3(cameraDir.x,0,cameraDir.z)), cameraUp)) * cameraSpeed;
                if(keys[GLFW_KEY_D])
                    position += glm::normalize(glm::cross(glm::normalize(glm::vec3(cameraDir.x,0,cameraDir.z)), cameraUp)) * cameraSpeed; 
            }
        }
        glm::mat4 get_view()
        {
            return glm::lookAt(position, cameraDir + position, cameraUp);
        }
        
    };
    class World : public GraphicObject, public SlowStateInterface //World/////////////////////////////
    {
        struct WorldKey //WorldKey///////////////////
        {
            int x;
            int z;
            WorldKey();
            WorldKey(int x, int z);
            bool operator<(const WorldKey &ob) const;
            bool operator==(const WorldKey &ob) const;
            bool operator!=(const WorldKey &ob) const;
            WorldKey& operator=(const WorldKey &ob);
            static WorldKey create_from_coordinates(glm::vec3 coordinates);
        };
        class Chunk : RenderInterface //Chunk//////////////////////
        {
            GLuint colorBO;
            WorldKey key;
        public:
            Chunk();
            Chunk(Shader& shader, WorldKey key);
            Chunk(const Chunk& other);
            bool init() override;
            void draw(glm::mat4 proj, glm::mat4 view) override;
            void set_color(glm::vec3 color);
        };
        // Chunk chunk;
        std::map<WorldKey, Chunk> chunks;
        WorldKey key_to_chunk_with_camera;
    public:
        World(Shader& shader);
        bool init();
        void draw(glm::mat4 proj, glm::mat4 view);
        void update();

    };
    class Save //Save//////////////////////////
    {

    };
    
    Camera camera;
    glm::mat4 proj;
    World *world = nullptr;

    public:
    static bool keys[];
    static const std::chrono::steady_clock::duration delta_time;

    GraphicContainer();
    ~GraphicContainer();
    void start();
    static void MoveMouse(GLFWwindow* window, double xpos, double ypos);
};
