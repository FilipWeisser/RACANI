#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 color[];
out vec3 g_color;

uniform mat4 u_projection_mat;
uniform mat4 u_model_mat;
uniform mat4 u_view_mat;
uniform vec3 u_eye;


void main() {
	
	vec4 v0 = u_model_mat * gl_in[0].gl_Position;
	vec4 v1 = u_model_mat * gl_in[1].gl_Position;
	vec4 v2 = u_model_mat * gl_in[2].gl_Position;

	vec3 center = vec3((v0 + v1 + v2)/3);
	vec3 e = u_eye - center;
	vec3 normal = cross(vec3(v1) - vec3(v0), vec3(v2) - vec3(v0));
	
	if(dot(normal, e) > 0) {
		gl_Position = u_projection_mat * u_view_mat * v0;
		g_color = color[0];
		EmitVertex();
		gl_Position = u_projection_mat * u_view_mat * v1;
		g_color = color[0];
		EmitVertex();
		gl_Position = u_projection_mat * u_view_mat * v2;
		g_color = color[0];
		EmitVertex();
		EndPrimitive();
	}
}