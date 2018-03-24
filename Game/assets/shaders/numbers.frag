#version 420

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
	float innerCutoff;
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

uniform Light light;

uniform Material material;
uniform int ammo;

in vec3 position;
in vec2 texCoord;
in vec3 normal;

out vec4 outColor;

vec3 calculateLight(Light light, vec3 norm, vec4 diff, vec4 spec) {
	float attenuation = 1.0;
	vec3 lightVec = light.position.xyz - position;
	float lightLen = length(lightVec);
	vec3 lightDir = normalize(lightVec);
	switch(light.type) {
	case SPOT:
		float spotDot = dot(normalize(light.direction.xyz), -lightDir);
		float spotValue = smoothstep(light.innerCutoff, light.cutoff, spotDot);
		attenuation = spotDot < light.cutoff ? light.partial : attenuation = pow(spotValue, light.spotExponent);
	case POINT:
		attenuation /= (light.attenuation[0] + light.attenuation[1] * lightLen + light.attenuation[2] * lightLen * lightLen);
		break;
	case DIRECT:
		lightDir = normalize(-light.direction).rgb;
		break;
	default:
		break;
	}

	// Ambient
	vec3 ambient = attenuation * light.ambient;
	// Diffuse
	float NdotL = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * NdotL * attenuation * diff.rgb;
	// Reflection
	vec3 reflectDir = reflect(-lightDir, norm);
	float VdotR = max(dot(normalize(-position), -reflectDir), 0.0);
	vec3 specular = light.specular * pow(VdotR, material.specExponent) * attenuation * spec.rgb;

	return ambient + diffuse + specular;
}

void main() {
	vec2 newCoord = { (texCoord.x + ammo % 10) / 10.0, texCoord.y };
	//vec2 newCoord = { texCoord.x, texCoord.y };

	// account for rasterizer interpolating
	vec3 norm = normalize(normal);	//normalize(texture(material.normal, newCoord).rgb);
	vec4 diff = texture(material.diffuse, newCoord);
	vec4 spec = texture(material.specular, newCoord);

	if (length(diff) >= 1.5)
		discard;

	outColor.rgb += calculateLight(light, norm, diff, spec);
	outColor.rgb *= material.hue;
	outColor.a = diff.a;
}
