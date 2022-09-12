#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include "shader.h"

/*
	directional light (eg sun)
*/

struct DirLight {
	// direction (constant for all parallel rays)
	glm::vec3 direction;

	// light values
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;

	// default constructor
	DirLight() = default;

	// constructor
	DirLight(glm::vec3 direction,
		glm::vec4 ambient,
		glm::vec4 diffuse,
		glm::vec4 specular)
		: direction(direction), ambient(ambient), diffuse(diffuse), specular(specular)
	{}

	// render light into shader
	void render(std::shared_ptr<shader>& shader_program);
};

/*
	point light (eg light bulb)
*/

struct PointLight {
	// position
	glm::vec3 position;

	// attenuation constants
	float k0;
	float k1;
	float k2;

	// light values
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;

	// default constructor
	PointLight() = default;

	// constructor
	PointLight(glm::vec3 position,
		float k0, float k1, float k2,
		glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular)
		: position(position), k0(k0), k1(k1), k2(k2), ambient(ambient), diffuse(diffuse), specular(specular)
	{
	}

	// render light into shader
	void render(int idx, std::shared_ptr<shader>& shader_program);
};

/*
	spot light (flashlight)
*/

struct SpotLight {
	// position
	glm::vec3 position;
	// direction
	glm::vec3 direction;

	// first level cut off
	float cutOff;
	// second level cut off
	float outerCutOff;

	// attenuation constants
	float k0;
	float k1;
	float k2;

	// light values
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;

	// default constructor
	SpotLight() = default;

	// constructor
	SpotLight(glm::vec3 position, glm::vec3 direction,
		float cutOff, float outerCutOff,
		float k0, float k1, float k2,
		glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular)
		: position(position), direction(direction), cutOff(cutOff), outerCutOff(outerCutOff), k0(k0), k1(k1), k2(k2), ambient(ambient), diffuse(diffuse), specular(specular)
	{}

	// render light into shader
	void render(int idx, std::shared_ptr<shader>& shader_program);
};

#endif