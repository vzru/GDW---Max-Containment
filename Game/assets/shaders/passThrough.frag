#version 420

in vec3 normal;

out vec4 outColor;

void main()
{
	outColor = vec4(normal, 1.0f);
}