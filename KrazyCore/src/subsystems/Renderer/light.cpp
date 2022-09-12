#include "light.h"

void DirLight::render(std::shared_ptr<shader>& shader_program)
{
	std::string name = "u_DirLight";

	shader_program->set_vec3(name + ".direction", direction);

	shader_program->set_vec4(name + ".ambient", ambient);
	shader_program->set_vec4(name + ".diffuse", diffuse);
	shader_program->set_vec4(name + ".specular", specular);
}

void PointLight::render(int idx, std::shared_ptr<shader>& shader_program)
{
	std::string name = "u_PointLights[" + std::to_string(idx) + "]";

	shader_program->set_vec3(name + ".position", position);

	shader_program->set_float(name + ".k0", k0);
	shader_program->set_float(name + ".k1", k1);
	shader_program->set_float(name + ".k2", k2);

	shader_program->set_vec4(name + ".ambient", ambient);
	shader_program->set_vec4(name + ".diffuse", diffuse);
	shader_program->set_vec4(name + ".specular", specular);
}

void SpotLight::render(int idx, std::shared_ptr<shader>& shader_program)
{
	std::string name = "u_SpotLights[" + std::to_string(idx) + "]";

	shader_program->set_vec3(name + ".position", position);
	shader_program->set_vec3(name + ".direction", direction);

	shader_program->set_float(name + ".cut_off_point", cutOff);
	shader_program->set_float(name + ".outer_cut_off_point", outerCutOff);

	shader_program->set_float(name + ".k0", k0);
	shader_program->set_float(name + ".k1", k1);
	shader_program->set_float(name + ".k2", k2);

	shader_program->set_vec4(name + ".ambient", ambient);
	shader_program->set_vec4(name + ".diffuse", diffuse);
	shader_program->set_vec4(name + ".specular", specular);
}
