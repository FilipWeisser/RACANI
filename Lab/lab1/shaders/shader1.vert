#version 330 core
layout (location = 0) in vec4 aPos;

out vec3 color;
uniform vec3 u_color;

void main()
{
    color = u_color;
    gl_Position = aPos;
}