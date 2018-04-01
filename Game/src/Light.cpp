#include "Light.h"

#include "Shader.h"

Light::Light()
{

}

Light::~Light()
{

}

void Light::sendUniforms(Shader* shader, glm::mat4 view, std::string prefix) {
	shader->sendUniform(prefix + "type", type);
	shader->sendUniform(prefix + "position", view * position);
	shader->sendUniform(prefix + "direction", view * direction);
	shader->sendUniform(prefix + "original", view * original);
	shader->sendUniform(prefix + "ambient", ambient);
	shader->sendUniform(prefix + "diffuse", diffuse);
	shader->sendUniform(prefix + "specular", specular);
	shader->sendUniform(prefix + "specExponent", specExponent);
	shader->sendUniform(prefix + "spotExponent", spotExponent);
	shader->sendUniform(prefix + "cutoff", cutoff);
	shader->sendUniform(prefix + "innerCutoff", innerCutoff);
	shader->sendUniform(prefix + "partial", partial);
	shader->sendUniform(prefix + "attenuation", attenuation);
}