#version 420

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;
uniform vec4 u_color;

uniform int animate;
uniform float t;

layout(location = 0) in vec3 in_vert;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normal;

layout(location = 3) in vec3 in_vert2;
layout(location = 4) in vec2 in_uv2;
layout(location = 5) in vec3 in_normal2;

out VertexData {
	vec3 position;
	vec2 texCoord;
	vec3 normal;
	vec4 color;
} vOut;

void main() {
	vec3 newVert = in_vert;
	vec3 newNormal = in_normal;
	vec2 newUV = in_uv;

	if (animate) {
		newVert = mix(in_vert, in_vert2, t);
		newNormal = mix(in_normal, in_normal2, t);
		newUV = mix(in_uv, in_uv2, t);
	}

	vec4 viewSpacePos = u_view * u_model * vec4(newVert, 1.f);
	vOut.position = viewSpacePos.xyz;
	vOut.normal = mat3(u_view * u_model) * newNormal;

	vOut.texCoord = newUV;
	gl_Position = u_proj * viewSpacePos;
}
