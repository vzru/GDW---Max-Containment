#version 420

#define NUM_LIGHTS 1

#define POINT 0
#define DIRECT 1
#define SPOT 2

struct Light {
	vec4 posDir;
	unsigned int type;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 attenuation;
};

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D normal;

	vec3 hue;
	float specularExponent;
};

uniform Light lights[NUM_LIGHTS];

uniform Material material;

in vec3 position;
in vec2 texCoord;
in vec3 normal;

out vec4 outColor;

vec3 calculateLight(Light light, vec3 norm, vec4 diff, vec4 spec);

void main() {
	// account for rasterizer interpolating
	vec3 norm = normalize(normal);//normalize(texture(material.normal, texCoord).rgb);
	vec4 diff = texture(material.diffuse, texCoord);
	vec4 spec = texture(material.specular, texCoord);

	for(int i = 0; i < NUM_LIGHTS; i++) {
		outColor.rgb += calculateLight(lights[i], norm, diff, spec);
	}

	outColor.rgb *= material.hue;
	outColor.a = diff.a;
}

vec3 calculateLight(Light light, vec3 norm, vec4 diff, vec4 spec) {
	vec3 lightDir;
	float attenuation = 1.0;
	switch(light.type) {
	case POINT:
		vec3 lightVec = light.posDir.xyz - position;
		float dist = length(lightVec);
		lightDir = lightVec / dist;
	
		// Attenuation (falloff)
		attenuation /= (light.attenuation[0] + light.attenuation[1] * dist + light.attenuation[2] * dist * dist);
		break;
	case DIRECT:
		lightDir = normalize(-light.posDir).rgb;
		break;
	case SPOT:
		break;
	default:
		break;
	}

	// Ambient
	vec3 ambient = attenuation * light.ambient;

	// Diffuse
	float NdotL = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = NdotL * attenuation * light.diffuse * diff.rgb;

	// Reflection
	vec3 reflectDir = reflect(-lightDir, norm);
	float VdotR = max(dot(normalize(-position), -reflectDir), 0.0);
	vec3 specular = light.specular * pow(VdotR, material.specularExponent) * attenuation * spec.rgb;
	
	return ambient + diffuse + specular;
}

/*
#version 420

#define NUM_POINT_LIGHTS 2

struct PointLight
{
	vec4 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
};
struct DirectionalLight
{
	vec4 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	sampler2D normal;

	vec3 hue;
	float specularExponent;
};

uniform PointLight pointLights[NUM_POINT_LIGHTS];

uniform DirectionalLight directionalLight;

uniform Material material;

in vec3 position;
in vec2 texCoord;
in vec3 normal;

out vec4 outColor;

vec3 calculatePointLight(PointLight light, vec3 norm, vec4 materialDiffuse, vec4 materialSpecular);
vec3 calculateDirectionalLight(DirectionalLight light, vec3 norm, vec4 materialDiffuse, vec4 materialSpecular);

void main()
{
	// account for rasterizer interpolating
	vec3 norm = normalize(texture(material.normal, texCoord).rgb);

	vec4 diffuse = texture(material.diffuse, texCoord);
	vec4 specular = texture(material.specular, texCoord);

	outColor.rgb = calculateDirectionalLight(directionalLight, norm, diffuse, specular);

	for(int i = 0; i < NUM_POINT_LIGHTS; i++)
	{
		outColor.rgb += calculatePointLight(pointLights[i], norm, diffuse, specular);
	}

	outColor.rgb *= material.hue;
	outColor.a = diffuse.a;
}

vec3 calculatePointLight(PointLight light, vec3 norm, vec4 materialDiffuse, vec4 materialSpecular)
{
	vec3 lightVec = light.position.xyz - position;
	float dist = length(lightVec);
	vec3 lightDir = lightVec / dist;

	// Attenuation (falloff)
	float attenuation = 1.0 / (light.constantAttenuation + light.linearAttenuation * dist + light.quadraticAttenuation * dist * dist);

	// Ambient
	vec3 ambient = attenuation * light.ambient;

	// Diffuse
	float NdotL = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = NdotL * attenuation * light.diffuse * materialDiffuse.rgb;

	// Specular
	vec3 reflectDir = reflect(-lightDir, norm);
	float VdotR = max(dot(normalize(-position), -reflectDir), 0.0);
	vec3 specular = light.specular * pow(VdotR, material.specularExponent) * attenuation * materialSpecular.rgb;

	//outColor.rgb += lightSpecular * pow(NdotHV, lightSpecularExponent) * attenuation;
	
	return ambient + diffuse + specular;
}

vec3 calculateDirectionalLight(DirectionalLight light, vec3 norm, vec4 materialDiffuse, vec4 materialSpecular)
{
	vec3 lightDir = normalize(-light.direction).rgb;

	// Ambient
	vec3 ambient = light.ambient;

	// Diffuse
	float NdotL = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = NdotL * light.diffuse * materialDiffuse.rgb;

	// Specular
	vec3 reflectDir = reflect(-lightDir, norm);
	float VdotR = max(dot(normalize(-position), -reflectDir), 0.0);
	vec3 specular = light.specular * pow(VdotR, material.specularExponent) * materialSpecular.rgb;

	return ambient + diffuse + specular;
}
*/