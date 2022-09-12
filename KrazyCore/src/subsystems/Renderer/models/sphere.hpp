#pragma once
#include <subsystems/Renderer/model.h>
#include <subsystems/Renderer/models/model_list.hpp>

class sphere : public model
{
public:
	sphere(const glm::vec3& pos = glm::vec3(0.0f), const glm::vec3& scale = glm::vec3(1.0f))
	: model(pos, scale, true)
	{}

	void init()
	{
		load("assets/models/sphere/scene.gltf");
	}
};

class sphere_list : public model_list<sphere>
{
public:

	void init()
	{
		m_model = sphere(glm::vec3(0.0f), glm::vec3(0.1f));
		m_model.init();
	}
};