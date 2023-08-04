#pragma once
#include <chrono>
#include <thread>
#include <map>
#include <vector>

#include "graphic_objects/graphic_object.hpp"
#include "graphic_objects/test_object.hpp"
#include "graphic_objects/slow_state_interface.hpp"

#define CHUNK_SIZE 10

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

class GraphicContainer
{
    class Settings //Settings/////////////////////////
    {
    public:
        int width;
        int height;
        GLFWwindow* window;
        Settings()
        {
            width = 800;
            height = 600;
            glfwInit();
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            window = glfwCreateWindow(width, height, "GraphicApp", NULL, NULL);

            glfwMakeContextCurrent(window);
            glfwSetKeyCallback(window, key_callback);
            gladLoadGL();
            glEnable(GL_DEPTH_TEST);
        }
    };
    static Settings settings;
    static Shader default_shader;

    class Camera : public StateInterface //Camera///////////////////////////
    {
        
        public:
        glm::vec3 cameraPos;
	    glm::vec3 cameraDir;
	    glm::vec3 cameraUp;
        glm::vec3 cameraRight;
	    GLfloat cameraSpeed = 0.05f;
	    glm::mat4x4 view;
        
        Camera() : Camera(glm::vec3(0.0f,2.0f,3.0f), glm::vec3(0.0f,2.0f,0.0f))
        {

        }
        Camera(glm::vec3 pos, glm::vec3 target)
        {
            cameraPos = pos;
            cameraDir = glm::normalize(target - pos);
            glm::vec3 up = glm::vec3(0,1,0);
            cameraRight = glm::normalize(glm::cross(up, cameraDir));
            cameraUp = glm::normalize(glm::cross(cameraDir, cameraRight));
            view = glm::lookAt(cameraPos, cameraDir + cameraPos, cameraUp);
        }
        void update() override
        {
            if(keys[GLFW_KEY_W])
                cameraPos += cameraSpeed * cameraDir;
            if(keys[GLFW_KEY_S])
                cameraPos -= cameraSpeed * cameraDir;
            if(keys[GLFW_KEY_A])
                cameraPos -= glm::normalize(glm::cross(cameraDir, cameraUp)) * cameraSpeed;
            if(keys[GLFW_KEY_D])
                cameraPos += glm::normalize(glm::cross(cameraDir, cameraUp)) * cameraSpeed; 
        }
        glm::mat4 get_view()
        {
            return glm::lookAt(cameraPos, cameraDir + cameraPos, cameraUp);
        }
    };
    class World : public GraphicObject, public SlowStateInterface //World/////////////////////////////
    {
        struct WorldKey //WorldKey///////////////////
        {
            int x;
            int z;
            WorldKey(int x, int z);
            bool operator<(const WorldKey &ob) const;
            bool operator==(const WorldKey &ob) const;
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
        };
        // Chunk chunk;
        std::map<WorldKey, Chunk> chunks;
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
};