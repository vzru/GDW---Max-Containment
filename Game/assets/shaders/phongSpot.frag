#version 420

#define MAX_NUM_LIGHTS 32

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

	// Reflection
	//vec3 reflectDir = reflect(-lightDir, norm);
	//float VdotR = max(dot(normalize(-position), -reflectDir), 0.0);
	switch(light.type) {
	case SPOT:
<<<<<<< HEAD
		//attenuation = 0.0;
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
		//return vec3(thetra);
		//if (thetra < light.cutoff)	{
		//	attenuation = light.partial;
		//}
		//else {
		//float NdotL = max(dot(norm, normalize(lightDir)),0.0);
		//if(NdotL > 0.0f)
		//{
			//if(thetra > light.cutoff)	{
			//	float epsilon = (-light.cutoff + light.outerCutoff);
			//	intensity = clamp((thetra - light.outerCutoff) / epsilon, 0.0, 1.0);
				//intensity = clamp((thetra), 0.0, 1.0); 
				//float spotValue = smoothstep(light.outerCutoff, light.cutoff, spotDot);
				//attenuation = pow(spotValue, light.spotExponent);
			//}
		//}
		break;
=======
		float spotDot = dot(normalize(light.direction.xyz), -lightDir);
		float spotValue = smoothstep(light.innerCutoff, light.cutoff, spotDot);
		attenuation = (spotDot < light.cutoff) ? light.partial : attenuation = pow(spotValue, light.spotExponent);
>>>>>>> Flashlight
	case POINT:
		attenuation /= (light.attenuation[0] + light.attenuation[1] * lightLen + light.attenuation[2] * lightLen * lightLen);
		break;
	case DIRECT:
		lightDir = normalize(-light.direction).rgb;
		break;
	default:
		break;
	}

<<<<<<< HEAD
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
=======
	// Ambient
	vec3 ambient = attenuation * light.ambient;
	// Diffuse
	float NdotL = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * NdotL * attenuation * diff.rgb;
	// Reflection (Phong)
	//vec3 reflectDir = reflect(-lightDir, norm);
	//float VdotR = max(dot(normalize(-position), -reflectDir), 0.0);
	//vec3 specular = light.specular * pow(VdotR, material.specExponent) * attenuation * spec.rgb;
	// Half-vector (Blinn-Phong)
	float NdotHV = max(dot(norm, normalize(lightDir + normalize(-position))), 0.0);
	vec3 specular = light.specular * pow(NdotHV, material.specExponent) * attenuation * spec.rgb;
>>>>>>> Flashlight

	return ambient + diffuse + specular;
}

void main() {
	// account for rasterizer interpolating
	vec3 norm = normalize(texture(material.normal, texCoord).rgb * 2 - vec3(1.0));
	vec4 diff = texture(material.diffuse, texCoord);
	vec4 spec = texture(material.specular, texCoord);

	for(int i = 0; i < numLights; i++) {
		outColor.rgb += calculateLight(lights[i], norm, diff, spec);
	}

	outColor.rgb *= material.hue;
	outColor.a = diff.a;
}
