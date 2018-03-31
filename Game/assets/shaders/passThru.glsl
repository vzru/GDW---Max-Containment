 #version 420

// Vertex Shader Inputs
// These are the attributes of the vertex
layout(location = 0) in vec3 in_vertex;
layout(location = 2) in vec2 in_uv;
layout(location = 1) in vec3 in_normal;

// Uniforms
// Constants throughout the entire pipeline
// These values are sent from C++ (glSendUniform*)
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;
uniform vec4 u_color;

out VertexData {
	vec3 position;
	vec2 texCoord;
	vec3 normal;
	vec4 color;
} vOut;

void main() 
{
	vOut.texCoord = in_uv;
	vOut.normal = in_normal;
	vOut.color = u_color;

	gl_Position = vec4(in_vertex, 1.0);
}