#version 420

#define NUM_LIGHTS 1

#define POINT 0
#define DIRECT 1
#define SPOT 2

#define UP_COLOR vec3(0, 0, 1)
#define DOWN_COLOR vec3(1, 0, 0)
#define LEFT_COLOR vec3(1, 0, 1)
#define RIGHT_COLOR vec3(0, 1, 0)
#define FRONT_COLOR vec3(0, 1, 1)
#define BACK_COLOR vec3(1, 1, 0)

struct Light {
	uint type;

	vec4 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float specExponent;

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
in vec3 modelNormal;

out vec4 outColor;

vec3 calculateLight(Light light, vec3 norm, vec4 diff, vec4 spec);

void main() {
	// account for rasterizer interpolating
	vec3 norm = normalize(texture(material.normal, texCoord).rgb);
	vec4 diff = texture(material.diffuse, texCoord);
	vec4 spec = texture(material.specular, texCoord);

	for(int i = 0; i < NUM_LIGHTS; i++) {
		outColor.rgb += calculateLight(lights[i], norm, diff, spec);
	}

	vec3 color = max(modelNormal.x, 0.0) * RIGHT_COLOR + max(-modelNormal.x, 0.0) * LEFT_COLOR +
		max(modelNormal.y, 0.0) * UP_COLOR + max(-modelNormal.y, 0.0) * DOWN_COLOR +
		max(modelNormal.z, 0.0) * FRONT_COLOR + max(-modelNormal.z, 0.0) * BACK_COLOR;
	outColor.rgb *= color;
	outColor.a = 1.0;
}

vec3 calculateLight(Light light, vec3 norm, vec4 diff, vec4 spec) {
	vec3 lightVec = light.position.xyz - position;
	float dist = length(lightVec);
	vec3 lightDir = lightVec / dist;

	//if (dist > 0.0)	{

		// Attenuation (falloff)
		float attenuation = 1.0 / (light.attenuation[0] + light.attenuation[1] * dist + light.attenuation[2] * dist * dist);

		// Ambient
		vec3 ambient = attenuation * light.ambient;

		// Diffuse
		float NdotL = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = NdotL * attenuation * light.diffuse;

		// Specular
		vec3 reflectDir = reflect(-lightDir, norm);
		float VdotR = max(dot(normalize(-position), -reflectDir), 0.0);
		vec3 specular = light.specular * pow(VdotR, light.specExponent) * attenuation;

		return ambient + diffuse + specular;
	//}
}
