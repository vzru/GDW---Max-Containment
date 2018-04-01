#version 420

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float specExponent;
	vec3 attenuation;
};

uniform Light light;
uniform vec4 objectColor;

in VertexData {
	vec3 position;
	vec2 texCoord;
	vec3 normal;
	vec4 color;
} vIn;

out vec4 outColor;

vec3 calculateLight(Light light) {
	// account for rasterizer interpolating
	vec3 norm = normalize(vIn.normal);

	vec3 lightVec = light.position.xyz - vIn.position;
	float dist = length(lightVec);
	vec3 lightDir = lightVec / dist;

	// whether light contributes to this surface
	float NdotL = dot(norm, lightDir);

	// Attenuation (falloff)
	float attenuation = 1.0 / (light.attenuation[0] + light.attenuation[1] * dist + light.attenuation[2] * dist * dist);

	// Ambient
	vec3 ambient = attenuation * light.ambient;

	// Diffuse
	vec3 diffuse = light.diffuse * NdotL * attenuation;

	// Blinn-Phong half vector
	float NdotHV = max(dot(norm, normalize(lightDir + normalize(-vIn.position))), 0.0);
	vec3 specular = light.specular * pow(NdotHV, light.specExponent) * attenuation;

	if (NdotL > 0.0)	return ambient + diffuse + specular;
	else				return ambient;
}

void main() {
	outColor.rgb = objectColor.rgb;

	outColor.rgb += calculateLight(light);

	outColor.a = objectColor.a;
}