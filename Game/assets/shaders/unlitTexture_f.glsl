#version 420

// Specify the texture unit directly in the shader
layout(binding = 0) uniform sampler2D u_rgb; // rgb texture

// Fragment Shader Inputs
in VertexData {
	vec3 position;
	vec2 texCoord;
	vec3 normal;
	vec4 color;
} vIn;

layout(location = 0) out vec4 FragColor;

void main() {
	vec4 color = texture(u_rgb, vIn.texCoord);
	FragColor = vec4(color.rgb, 1.0);
}