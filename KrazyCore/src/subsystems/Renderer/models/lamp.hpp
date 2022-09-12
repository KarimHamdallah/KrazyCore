#pragma once
#include "cube.hpp"
#include <subsystems/Renderer/light.h>
#include <subsystems/Renderer/models/model_list.hpp>

class lamp : public cube
{
public:
	glm::vec3 lightcolor;
	PointLight pointlight;

	lamp() = default;
	lamp(
		glm::vec3 light_color,
		glm::vec3 position,
		glm::vec3 scale,
		glm::vec4 ambient,
		glm::vec4 diffuse,
		glm::vec4 specular,
		f32 k0,
		f32 k1,
		f32 k2
		)
		: cube(position, scale), lightcolor(light_color), pointlight(position, k0, k1, k2, ambient, diffuse, specular)
	{
	}

	void render(const std::shared_ptr<shader>& shader_program, bool set_model_matrix = true, bool do_draw_call = true) override
	{
		shader_program->set_vec3("u_LightColor", lightcolor);
		cube::render(shader_program);
	}
};

class lamp_list : public model_list<lamp>
{
public:
	std::vector<PointLight> lights;

	void init()
	{
		m_model = lamp(
			glm::vec3(1.0f),
			glm::vec3(0.0f),
			glm::vec3(0.25f),
			glm::vec4(0.05f, 0.05f, 0.05f, 1.0f),
			glm::vec4(0.8f, 0.8f, 0.8f, 1.0f),
			glm::vec4(1.0f),
			1.0f, 0.07f, 0.32f);

		model_list::init();
	}

	void render(const std::shared_ptr<shader>& shader)
	{
		positions.clear();
		scales.clear();

		for (PointLight& light : lights)
		{
			positions.push_back(light.position);
			scales.push_back(m_model.scale);
		}
		
		model_list::render(shader, false);
	}
};