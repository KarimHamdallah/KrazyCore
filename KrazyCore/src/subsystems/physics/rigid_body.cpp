#include "rigid_body.h"
#include <core/Application.h>

rigid_body::rigid_body()
	: mass(1.0f), rb_pos(glm::vec3(0.0f)), rb_velocity(glm::vec3(0.0f)), rb_acceleration(glm::vec3(0.0f))
{
}

rigid_body::rigid_body(f32 mass, glm::vec3 position)
	: mass(mass), rb_pos(position), rb_velocity(glm::vec3(0.0f)), rb_acceleration(glm::vec3(0.0f))
{
}

rigid_body::~rigid_body()
{
}

void rigid_body::update()
{
	f32 deltaTime = (f32)DELTA_TIME;
	rb_pos += rb_velocity * deltaTime + (0.5f * rb_acceleration) * (deltaTime * deltaTime);
	rb_velocity += rb_acceleration * deltaTime;
}

void rigid_body::add_impulse(glm::vec3 dir, f32 magnitude)
{
	f32 deltaTime = (f32)DELTA_TIME;
	glm::vec3 impulse = glm::normalize(dir) * magnitude;
	rb_velocity += (impulse / mass) * deltaTime;
}

void rigid_body::add_acceleration(glm::vec3 a)
{
	rb_acceleration += a;
}