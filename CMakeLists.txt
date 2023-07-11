cmake_minimum_required(VERSION 3.0)

find_package(OpenGL REQUIRED)

project(GraphicApp CXX)

add_executable(${PROJECT_NAME} main.cpp)

add_subdirectory(external/glfw)
add_subdirectory(external/glad)


target_include_directories(${PROJECT_NAME} 
    PUBLIC external/glfw/include
)
target_link_directories(${PROJECT_NAME}
    PRIVATE external/glfw/src
)

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARY} glfw glad)