#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 2) out;

in vec3 color[];
out vec3 g_color;

void main() {
	gl_Position = gl_in[0].gl_Position;
	g_color = color[0];
	EmitVertex();
	
	EndPrimitive();
	
}
