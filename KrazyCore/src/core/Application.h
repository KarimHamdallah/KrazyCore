#pragma once
#include <defines.h>
#include <string>
#include <subsystems/Renderer/mesh.h>
#include <gameplay/fps_player.h>
#include <subsystems/Renderer/light.h>
#include <subsystems/Renderer/models/lamp.hpp>
#include <subsystems/Renderer/models/sphere.hpp>

class window;

class Application
{
public:
	Application(std::string title, u32 width, u32 height);
	~Application();


	void run();
	void stop();



	inline static Application* get_Instance() { return m_instance; }

	inline u32 get_width()const { return m_width; };
	inline u32 get_height()const { return m_height; };

	inline void set_width(u32 width) { m_width = width; }
	inline void set_height(u32 height) { m_height = height; }

	inline const char* get_title()const { return m_title; }

	std::shared_ptr<window> get_window()const { return m_window; }

	inline double get_delta_time() const { return m_delta_time; }

	// TODO::Remove
	std::shared_ptr<fps_player> m_player;
private:

	bool init();
	void shutdown();

	u32 m_width;
	u32 m_height;
	const char* m_title;

	bool m_running;
	bool m_suspended;

	static Application* m_instance;
	std::shared_ptr<window> m_window;


	// TODO:: Remove this
	std::shared_ptr<shader> shader_program;
	std::shared_ptr<shader> Lmap_shader_program;
	std::shared_ptr<shader> sphere_shader_program;
	
	DirLight m_dirLigh = {
		glm::vec3(-2.0f, -1.0f, -0.3f),
		glm::vec4(0.1f, 0.1f, 0.1f, 1.0f),
		glm::vec4(0.4f, 0.4f, 0.4f, 1.0f),
		glm::vec4(0.75f, 0.75f, 0.75f, 1.0f)
	};

	glm::vec3 m_pointLightPositions[4] = 
	{
	        glm::vec3(0.7f,  0.2f,  2.0f),
	        glm::vec3(2.3f, -3.3f, -4.0f),
	        glm::vec3(-4.0f,  2.0f, -12.0f),
	        glm::vec3(0.0f,  0.0f, -3.0f)
	};

	lamp m_lamps[4];

	SpotLight m_spotLight;

	//std::shared_ptr<sphere> m_sphere;

	sphere_list m_sphere_list;

	void throw_sphere();

	double m_delta_time = 0.0f;
	double m_last_frame_time = 0.0f;
};

#define DELTA_TIME Application::get_Instance()->get_delta_time()