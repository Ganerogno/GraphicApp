#include "graphic_container.hpp"

#include <glm/glm.hpp> // vec2, vec3, mat4, radians
#include <glm/ext.hpp> // perspective, translate, rotate

bool GraphicContainer::keys[91]{};
const std::chrono::steady_clock::duration GraphicContainer::delta_time = std::chrono::milliseconds(16);
GraphicContainer::Settings GraphicContainer::settings;
Shader GraphicContainer::default_shader = Shader();

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    
    if (key <= GLFW_KEY_Z)
    {
        if (action != GLFW_RELEASE)
        {
            GraphicContainer::keys[key] = true;
        }
        else
        {
            GraphicContainer::keys[key] = false;
        }
    }
}

GraphicContainer::World::WorldKey::WorldKey(int x, int z): x(x),z(z)
{
    
}
bool GraphicContainer::World::WorldKey::operator<(const WorldKey &ob) const {
    return x < ob.x || (x == ob.x && z < ob.z);
}
bool GraphicContainer::World::WorldKey::operator==(const WorldKey &ob) const {
    return x == ob.x && z == ob.z;
}

GraphicContainer::World::Chunk::Chunk() : RenderInterface(default_shader), key(0,0){}
GraphicContainer::World::Chunk::Chunk(Shader& shader, WorldKey key) : RenderInterface(shader), key(key) {}
GraphicContainer::World::Chunk::Chunk(const Chunk& other) : RenderInterface(other.shader), key(other.key)
{
    EBO = other.EBO;
    VBO = other.VBO;
    VAO = other.VAO;
    colorBO = other.colorBO;
    model = other.model;
    indicies_count = other.indicies_count;
}
bool GraphicContainer::World::Chunk::init()
{
    glm::vec3 vertices[CHUNK_SIZE * CHUNK_SIZE]{};
    for(int i = 0; i < CHUNK_SIZE; i++)
    {
        for(int j = 0; j < CHUNK_SIZE; j++)
        {
            vertices[i * CHUNK_SIZE + j] = glm::vec3(i + key.x * (CHUNK_SIZE - 1),0,j + key.z * (CHUNK_SIZE - 1));
        }
    }
    std::vector<GLuint> indices;
    for(int i = 0; i < CHUNK_SIZE - 1; i++)
    {
        for(int j = 0; j < CHUNK_SIZE - 1; j++)
        {
            indices.push_back(i * CHUNK_SIZE + j);
            indices.push_back(i * CHUNK_SIZE + j + 1);
            indices.push_back((i + 1) * CHUNK_SIZE + j);
            
            indices.push_back((i + 1) * CHUNK_SIZE + j + 1);
            indices.push_back(i * CHUNK_SIZE + j + 1);
            indices.push_back((i + 1) * CHUNK_SIZE + j);
        }
    }
    std::vector<GLfloat> colors;
    for(int i = 0; i < CHUNK_SIZE; i++)
    {
        for(int j = 0; j < CHUNK_SIZE; j++)
        {
            colors.push_back(0.1 + rand()%21/(float)100);
            colors.push_back(0.8 + rand()%21/(float)100);
            colors.push_back(0.2 + rand()%21/(float)100);
        }
    }
    indicies_count = indices.size();
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &colorBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLfloat), &indices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

    glBindBuffer(GL_ARRAY_BUFFER, colorBO); 
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), &colors[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    model = glm::mat4(1.0f);
    return true;
}
void GraphicContainer::World::Chunk::draw(glm::mat4 proj, glm::mat4 view)
{
    GLint modelLoc = glGetUniformLocation(shader.GetProgram(), "model");
    GLint projLoc = glGetUniformLocation(shader.GetProgram(), "projection");
    GLint viewLoc = glGetUniformLocation(shader.GetProgram(), "view");
    GLint colorLoc = glGetUniformLocation(shader.GetProgram(), "in_color");
    shader.Use();
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indicies_count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    shader.UseBaseShader();
}
GraphicContainer::World::World(Shader& shader) : GraphicObject(shader)
{
    for(int i = -2; i <= 2 ; i++)
    {
        for(int j = -2; j <= 2 ; j++)
        {
            chunks.emplace(WorldKey(i,j),Chunk(shader,WorldKey(i,j)));
        }
    }
}
// Chunk* GraphicContainer::World::get_chunk(int x, int z)
// {
//     // WorldKey key(x/Chunk::CHUNK_SIZE + x < 0 ? -1 : 1,z/Chunk::CHUNK_SIZE + z < 0 ? -1 : 1);
//     WorldKey key(0,0);
//     return &chunks[key];
// }
bool GraphicContainer::World::init()
{
    // chunk.init();
    // chunks.at(WorldKey(0,0)).init();
    // chunks.insert( {WorldKey(1,0), Chunk()});
    // chunks[WorldKey(0,0)].init();
    for(std::map<WorldKey,Chunk>::iterator it = chunks.begin(); it != chunks.end(); it++)
    {
        it->second.init();
    }
    return true;
}
void GraphicContainer::World::draw(glm::mat4 proj, glm::mat4 view)
{
    // chunk.draw(proj, view);
    // chunks[WorldKey(0,0)].draw(proj, view);
    for(std::map<WorldKey,Chunk>::iterator it = chunks.begin(); it != chunks.end(); it++)
    {
        it->second.draw(proj, view);
    }
}
void GraphicContainer::World::update()
{

}
GraphicContainer::GraphicContainer()
{
    
}
GraphicContainer::~GraphicContainer()
{
    delete world;
}

void GraphicContainer::start()
{
    proj = glm::perspective( 45.0f, (GLfloat)settings.width/(GLfloat)settings.height, 0.1f, 100.0f);
    glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    Shader testShader("test",true,true);
    Shader worldShader("world",true,true);
    TestObject object(testShader);
    object.init();
    world = new World(worldShader);
    world->init();
    glClearColor(0.2f, 0.1f, 0.5f, 1.0f);
    std::chrono::steady_clock::time_point start_time;
    while (!glfwWindowShouldClose(settings.window)) 
    {
        start_time = std::chrono::steady_clock::now();
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        camera.update();
        object.draw(proj,camera.get_view());
        world->draw(proj,camera.get_view());
        glfwSwapBuffers(settings.window);
        if (std::chrono::steady_clock::now() < start_time + delta_time)
        {
            std::this_thread::sleep_until(start_time + delta_time);
        }
    }
}