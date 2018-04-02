#include "Light.h"
#include "Shader.h"

Light::Light() {}

Light::~Light() {}

void Light::sendUniforms(Shader* shader, glm::mat4& view, std::string prefix) {
	shader->sendUniform(prefix + "type", type)
		->sendUniform(prefix + "position", view * position)
		->sendUniform(prefix + "direction", view * direction)
		->sendUniform(prefix + "original", view * original)
		->sendUniform(prefix + "ambient", ambient)
		->sendUniform(prefix + "diffuse", diffuse)
		->sendUniform(prefix + "specular", specular)
		->sendUniform(prefix + "specExponent", specExponent)
		->sendUniform(prefix + "spotExponent", spotExponent)
		->sendUniform(prefix + "cutoff", cutoff)
		->sendUniform(prefix + "outerCutoff", outerCutoff)
		->sendUniform(prefix + "partial", partial)
		->sendUniform(prefix + "attenuation", attenuation);
}