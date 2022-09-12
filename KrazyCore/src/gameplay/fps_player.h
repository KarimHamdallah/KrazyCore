#pragma once
#include <subsystems/Renderer/camera3d.h>
#include <memory>

class fps_player
{
public:
	fps_player(const glm::vec3& position);

	void update();

	glm::vec3 position;

	std::shared_ptr<camera3d> get_camera() const { return m_camera; }
private:
	std::shared_ptr<camera3d> m_camera;
};