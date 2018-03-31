#version 420

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

layout(location = 0) in vec3 in_vert;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normal;

out VertexData {
	vec3 position;
	vec2 texCoord;
	vec3 normal;
} vOut;

void main() {
	vOut.texCoord = in_uv;
	vOut.normal = mat3(uView * uModel) * in_normal;
	
	vec4 viewSpacePos = uView * uModel * vec4(in_vert, 1.f);
	vOut.position = viewSpacePos.xyz;
	gl_Position = uProj * viewSpacePos;
}