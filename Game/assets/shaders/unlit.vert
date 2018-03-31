#version 420

uniform mat4 uModel;
uniform mat4 uMView;
uniform mat4 uMProj;

layout(location = 0) in vec3 in_vert;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normal;

//layout(location = 3) in vec3 in_vert;
//layout(location = 4) in vec2 in_uv;
//layout(location = 5) in vec3 in_normal;

out vec3 position;
out vec2 texCoord;
out vec3 normal;

void main() {
	vec4 viewSpacePos = uMView * vec4(in_vert, 1.f);
	position = viewSpacePos.xyz;
	texCoord = in_uv;
	normal = (uMView * vec4(in_normal, 1.0f)).xyz;
	
	gl_Position = uMProj * vec4(in_vert, 1.f);
}