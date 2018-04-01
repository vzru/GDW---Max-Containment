#version 420

// Specify the texture unit directly in the shader
layout(binding = 0) uniform sampler2D u_rgb; // rgb texture

uniform vec3 color;

// Fragment Shader Inputs
in VertexData {
	vec3 position;
	vec2 texCoord;
	vec3 normal;
	vec4 color;
} vIn;

layout(location = 0) out vec4 FragColor;

void main()
{
	//vec4 color = texture(u_rgb, vIn.texCoord.xy);
	FragColor = vec4(color, 1.0f);//color.xyz, 1.0);
	vec4 color = texture(u_rgb, vIn.texCoord);
}