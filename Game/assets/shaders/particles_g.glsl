// Geometry shader which turns points into billboarded quads

#version 420

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;
uniform vec4 u_color;
uniform float size;

// Input from Vertex shader
in VertexData {
	vec3 position;
	vec2 texCoord;
	vec3 normal;
	vec4 color;
} vIn[]; // array size = num vertices in primitve

// Output from geometry shader
out VertexData {
	vec3 position;
	vec2 texCoord;
	vec3 normal;
	vec4 color;
} vOut; // array size depends on max_vertices

// Creates a quad of specified size around point p
void PointToQuadBillboarded(vec4 p, float size) {
	float halfSize = size * 0.5;

	vec4 pEye = u_model * u_view * p;

	vec4 topLeft = vec4(pEye.xy + vec2(-halfSize, halfSize), pEye.z, 1.0f);
	vec4 bottomLeft = vec4(pEye.xy + vec2(-halfSize, -halfSize), pEye.z, 1.0f);
	vec4 topRight = vec4(pEye.xy + vec2(halfSize, halfSize), pEye.z, 1.0f);
	vec4 bottomRight = vec4(pEye.xy + vec2(halfSize, -halfSize), pEye.z, 1.0f);

	gl_Position = u_proj * bottomLeft;
	vOut.texCoord.xy = { 0.f, 0.f };
	vOut.color = vIn[0].color;
	EmitVertex();

	gl_Position = u_proj * topLeft;
	vOut.texCoord.xy = { 0.f, 1.0f };
	vOut.color = vIn[0].color;
	EmitVertex();

	gl_Position = u_proj * bottomRight;
	vOut.texCoord.xy = { 1.0f, 0.0f };
	vOut.color = vIn[0].color;
	EmitVertex();

	gl_Position = u_proj * topRight;
	vOut.texCoord.xy = { 1.0f, 1.0f };
	vOut.color = vIn[0].color;
	EmitVertex();

	EndPrimitive();
}

void main() {
	PointToQuadBillboarded(gl_in[0].gl_Position, size);
}




