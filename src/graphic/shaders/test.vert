#version 460 core

layout (location = 0) in vec3 position;

void main()
{
    gl_Position = vec4(position.x/2, position.y/2, position.z/2, 1.0);
    
}