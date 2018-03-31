#version 420

#define NUM_LIGHTS 1
#define MAX_NUM_LIGHTS 128

#define POINT 0
#define DIRECT 1
#define SPOT 2

struct Light {
	uint type;

	vec4 position;
	vec4 direction;
	vec4 original;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float specExponent;
	float spotExponent;
	float outerCutoff;
	float cutoff;
	float partial;

	vec3 attenuation;
};

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D normal;

	vec3 hue;
	float specExponent;
};

uniform Light lights[NUM_LIGHTS];

uniform Material material;

in vec3 position;
in vec2 texCoord;
in vec3 normal;

out vec4 outColor;

void main() {
	// account for rasterizer interpolating
	vec3 norm = normalize(normal);//normalize(texture(material.normal, texCoord).rgb);
	vec4 diff = texture(material.diffuse, texCoord);
	vec4 spec = texture(material.specular, texCoord);

	//for(int i = 0; i < NUM_LIGHTS; i++) {
	//	outColor.rgb += calculateLight(lights[i], norm, diff, spec);
	//}

	outColor.rgb = vec3(1.0f, 0.f, 0.f);//diff.rgb;// + spec;

	//outColor.rgb *= material.hue;
	outColor.a = 1.0f;//diff.a;
}
