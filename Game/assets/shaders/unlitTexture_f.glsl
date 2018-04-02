#version 420

// Specify the texture unit directly in the shader
//layout(binding = 0) uniform sampler2D u_rgb; // rgb texture

struct Material {
	layout(binding = 0) sampler2D diffuse;
};

uniform Material material;

uniform vec4 u_color;

// Fragment Shader Inputs
in VertexData {
	vec3 position;
	vec2 texCoord;
	vec3 normal;
	vec4 color;
} vIn;

layout(location = 0) out vec4 FragColor;

void main() {
	FragColor = vec4(0,0,1,1);//texture(material.diffuse, vIn.texCoord);
	//FragColor = vec4(color, 1.0f);//color.xyz, 1.0);
}