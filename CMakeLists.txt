cmake_minimum_required(VERSION 3.2)

find_package(OpenGL REQUIRED)

project(GraphicApp CXX)

add_executable(${PROJECT_NAME} src/main.cpp)

add_subdirectory(external/glfw)
add_subdirectory(external/glad)
add_subdirectory(external/glm)


target_include_directories(${PROJECT_NAME} 
    PUBLIC external/glfw/include
)
target_link_directories(${PROJECT_NAME}
    PRIVATE external/glfw/src
    PUBLIC external/glm
)

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARY} glfw glad glm::glm)