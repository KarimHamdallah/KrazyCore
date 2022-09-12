#include "fps_player.h"
#include <subsystems/input/keyboard.h>
#include <subsystems/input/mouse.h>
#include <core/Application.h>

fps_player::fps_player(const glm::vec3& position)
	: position(position)
{
	m_camera = std::make_shared<camera3d>(position);
}

void fps_player::update()
{
	if (keyboard::key_pressed(GLFW_KEY_W))
		m_camera->update_position(camera_dir::FORWARD);
	if (keyboard::key_pressed(GLFW_KEY_S))
		m_camera->update_position(camera_dir::BACKWARD);
	if (keyboard::key_pressed(GLFW_KEY_D))
		m_camera->update_position(camera_dir::RIGHT);
	if (keyboard::key_pressed(GLFW_KEY_A))
		m_camera->update_position(camera_dir::LEFT);
	if (keyboard::key_pressed(GLFW_KEY_E))
		m_camera->update_position(camera_dir::UP);
	if (keyboard::key_pressed(GLFW_KEY_Q))
		m_camera->update_position(camera_dir::DOWN);

	double mouse_delta_x = mouse::get_dx();
	double mouse_delta_y = mouse::get_dy();

	f32 delta_time = Application::get_Instance()->get_delta_time();

	if (mouse::button_pressed(GLFW_MOUSE_BUTTON_2))
	{
		if (mouse_delta_x != 0 || mouse_delta_y != 0)
		{
			m_camera->update_direction(mouse_delta_x * delta_time * 2.5f, -mouse_delta_y * delta_time * 2.5f);
		}
	}
}