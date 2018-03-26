#version 420

#define MAX_NUM_LIGHTS 10

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

#define numLights 4

uniform Light lights[MAX_NUM_LIGHTS];

uniform Material material;

in vec3 position;
in vec2 texCoord;
in vec3 normal;

out vec4 outColor;

vec3 calculateLight(Light light, vec3 norm, vec4 diff, vec4 spec) {
	float attenuation = 1.0;
	float intensity = 0.0;
	vec3 lightVec = light.position.xyz - position;
	float lightLen = length(lightVec);
	vec3 lightDir = normalize(lightVec);
	float NdotL = max(dot(norm, lightDir), 0.0);
	float NdotHV = max(dot(norm, normalize(lightDir + normalize(-position))), 0.0);
	
	// Ambient
	vec3 ambient = light.ambient * diff.rgb;
	// Diffuse
	vec3 diffuse = light.diffuse * NdotL * diff.rgb;
	// Specular
	vec3 specular = light.specular * pow(NdotHV, material.specExponent) * spec.rgb;
	switch(light.type) {
	case SPOT:
		float thetra = dot(lightDir, normalize(-light.direction.xyz));
		thetra = clamp(thetra, 0.0, 1.0);
		thetra = thetra * 1.01;
		thetra = pow(thetra, 10.0);
		intensity = thetra;//clamp(thetra, 0.0, 1.0);
		diffuse *= intensity;
		specular *= intensity;
		attenuation = intensity;
		attenuation /= (	light.attenuation[0] + 
							light.attenuation[1] * lightLen + 
							light.attenuation[2] * lightLen * lightLen );
		break;
	case POINT:
		attenuation /= (light.attenuation[0] + light.attenuation[1] * lightLen + light.attenuation[2] * lightLen * lightLen);
		break;
	case DIRECT:
		lightDir = normalize(-light.direction).rgb;
		break;
	default:
		break;
	}
	
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;


	return ambient + diffuse + specular;
}

void main() {
	// account for rasterizer interpolating
	vec3 norm = normalize(normal);//normalize(texture(material.normal, texCoord).rgb * 2 - vec3(1.f));
	vec4 diff = texture(material.diffuse, texCoord);
	vec4 spec = texture(material.specular, texCoord);

	for(int i = 0; i < numLights; i++) {
		outColor.rgb += calculateLight(lights[i], norm, diff, spec);
	}

	outColor.rgb *= material.hue;
	outColor.a = diff.a;
}
