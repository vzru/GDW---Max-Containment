#version 420

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

uniform int animate;
uniform float t;

layout(location = 0) in vec3 in_vert;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normal;

layout(location = 3) in vec3 in_vert2;
layout(location = 4) in vec2 in_uv2;
layout(location = 5) in vec3 in_normal2;

out vec3 position;
out vec3 normal;
out vec2 texCoord;


float lerp(float d0, float d1, float dt)
{
	return (1 - dt) * d0 + d1 * dt;
}

void main() {
	vec3 newVert = in_vert;
	vec3 newNormal = in_normal;

	if(animate)
	{
		newVert = vec3(	lerp(in_vert.x, in_vert2.x, t),
								lerp(in_vert.y, in_vert2.y, t),
								lerp(in_vert.z, in_vert2.z, t)	);

		newNormal = vec3(	lerp(in_normal.x, in_normal2.x, t),
								lerp(in_normal.y, in_normal2.y, t),
								lerp(in_normal.z, in_normal2.z, t)	);
	}

	vec4 viewSpacePos = uView * uModel * vec4(newVert, 1.f);
	position = viewSpacePos.xyz;
	normal = mat3(uView * uModel) * newNormal;

	texCoord = in_uv;
	gl_Position = uProj * viewSpacePos;
}
