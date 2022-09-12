#pragma once
#include <defines.h>
#include <glm/glm.hpp>

class rigid_body
{
public:

	rigid_body();
	rigid_body(f32 mass, glm::vec3 psoition);
	~rigid_body();

	void update();
	void add_impulse(glm::vec3 dir, f32 magnitude);
	void add_acceleration(glm::vec3 a);

	f32 mass;
	glm::vec3 rb_pos;
	glm::vec3 rb_velocity;
	glm::vec3 rb_acceleration;
};