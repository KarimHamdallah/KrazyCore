#include "camera3d.h"
#include <glm/gtc/matrix_transform.hpp>
#include <core/Application.h>

camera3d::camera3d(const glm::vec3 & position)
	: m_pos(position), m_world_up(glm::vec3(0.0f,1.0f,0.0f)),m_yaw(-90.0f),m_pitch(0.0f),m_speed(1.5f),m_fov(45.0f)
	, m_camera_front(glm::vec3(0.0f,0.0f,-1.0f))
{
	update_camera_vectors();
}

camera3d::~camera3d()
{
}

void camera3d::update_direction(double dx, double dy)
{
	m_yaw += dx;
	m_pitch += dy;

	if (m_pitch > 89.0f)
		m_pitch = 89.0f;
	if (m_pitch < -89.0f)
		m_pitch = -89.0f;

	update_camera_vectors();
}

void camera3d::update_position(camera_dir dir)
{
	f32 speed = Application::get_Instance()->get_delta_time() * m_speed;

	switch (dir)
	{
	case camera_dir::FORWARD:
	{
		m_pos += m_camera_front * speed;
	}
	break;
	case camera_dir::BACKWARD:
	{
		m_pos -= m_camera_front * speed;
	}
	break;
	case camera_dir::UP:
	{
		m_pos += m_camera_up * speed;
	}
	break;
	case camera_dir::DOWN:
	{
		m_pos -= m_camera_up * speed;
	}
	break;
	case camera_dir::RIGHT:
	{
		m_pos += m_camera_right * speed;
	}
	break;
	case camera_dir::LEFT:
	{
		m_pos -= m_camera_right * speed;
	}
	break;
	default:
		break;
	}
}

glm::mat4 camera3d::get_view_matrix() const
{
	return glm::lookAt(m_pos, m_pos + m_camera_front, m_camera_up);
}

void camera3d::update_camera_vectors()
{
	glm::vec3 dir;
	dir.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	dir.y = sin(glm::radians(m_pitch));
	dir.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

	m_camera_front = glm::normalize(dir);

	m_camera_right = glm::normalize(glm::cross(m_camera_front, m_world_up));

	m_camera_up = glm::normalize(glm::cross(m_camera_right, m_camera_front));
}