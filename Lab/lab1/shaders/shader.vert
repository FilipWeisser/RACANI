#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 color;

uniform vec3 u_color;
uniform mat4 u_projection_mat;
uniform mat4 u_view_mat;

void main()
{
	color = u_color;
    gl_Position = u_projection_mat * u_view_mat * vec4(aPos,1);
}