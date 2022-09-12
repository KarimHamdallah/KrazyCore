#pragma once
#include "defines.h"
#include <glm/glm.hpp>

enum class camera_dir
{
	FORWARD = 0,
	BACKWARD,
	UP,
	DOWN,
	RIGHT,
	LEFT
};

class camera3d
{
public:
	camera3d() = default;
	camera3d(const glm::vec3& position);
	~camera3d();

	void update_direction(double dx, double dy);
	void update_position(camera_dir dir);
	glm::mat4 get_view_matrix()const;

	inline glm::vec3 get_position() const { return m_pos; }
	inline f32 get_fov() const { return m_fov; }
	inline glm::vec3 get_cameraFront()const { return m_camera_front; }
private:
	void update_camera_vectors();

	glm::vec3 m_pos;
	glm::vec3 m_camera_front;
	glm::vec3 m_camera_up;
	glm::vec3 m_camera_right;
	camera_dir m_dir;

	glm::vec3 m_world_up;

	f32 m_speed, m_fov;
	f32 m_yaw, m_pitch;
};