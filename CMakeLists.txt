cmake_minimum_required(VERSION 3.2)

set (CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)

project(GraphicApp CXX)

set(SOURCE 
    src/graphic/functions/shader.cpp
    src/graphic/graphic_objects/graphic_object.cpp
    src/graphic/graphic_objects/test_object.cpp
    src/graphic/graphic_container.cpp
    src/main.cpp)

set(HEADER 
    src/graphic/functions/shader.hpp
    src/graphic/graphic_objects/render_interface.hpp
    src/graphic/graphic_objects/slow_state_interface.hpp
    src/graphic/graphic_objects/slow_state_interface.hpp
    src/graphic/graphic_objects/graphic_object.hpp
    src/graphic/graphic_objects/test_object.hpp
    src/graphic/graphic_container.hpp)
add_executable(${PROJECT_NAME} ${HEADER} ${SOURCE}
    )

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