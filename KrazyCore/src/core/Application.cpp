#include "Application.h"
#include "subsystems/window.h"
#include "subsystems/asserts.h"
#include "subsystems/Renderer/renderer.h"
#include <subsystems/input/mouse.h>
#include <subsystems/physics/physics_world.h>
#include <vector>
#include <stack>


Application* Application::m_instance = nullptr;
static bool start = true;

Application::Application(std::string title, u32 width, u32 height)
	: m_title(title.c_str()),m_width(width),m_height(height),m_running(false),m_suspended(false),m_window(nullptr)
{
	m_running = init();
}

Application::~Application()
{
	shutdown();
}

void Application::run()
{
	KRAZY_DEBUG("Application is running");

	while (m_running)
	{
		// calculate delta time
		double current_frame_time = glfwGetTime();
		m_delta_time = current_frame_time - m_last_frame_time;
		m_last_frame_time = current_frame_time;

		if(!m_window->is_running())
			stop();


		if(!m_suspended)
		{
			m_window->update();

			m_player->update();

			render_command::clear_buffer();
			render_command::clear_color(93, 102, 95);

			//////////////////////////
			// view, projection matrices and view_pos vector
			// view and projection matrices
			glm::mat4 projection = glm::mat4(1.0f);
			glm::mat4 view = glm::mat4(1.0f);

			f32 aspect_ratio = (f32)m_width / (f32)m_height;

			projection = glm::perspective(glm::radians(m_player->get_camera()->get_fov()), aspect_ratio, 0.1f, 1000.0f);
			view = m_player->get_camera()->get_view_matrix();
			
			shader_program->bind();
			
			shader_program->set_mat4("Projection", projection);
			shader_program->set_mat4("View", view);
			shader_program->set_vec3("u_ViewPos", m_player->get_camera()->get_position());
			
			sphere_shader_program->bind();
			sphere_shader_program->set_vec3("u_ViewPos", m_player->get_camera()->get_position());
			///////////////////////////
			// Render models

			if (mouse::button_down(GLFW_MOUSE_BUTTON_1))
			{
				throw_sphere();
			}

			std::stack<int> removed_spheres;

			for (int i = 0; i < m_sphere_list.instances.size(); i++)
			{
				if (glm::length(m_player->get_camera()->get_position() - m_sphere_list.instances[i].rb_pos) > 150.0f)
				{
					removed_spheres.push(i);
					continue;
				}
			}

			for (int i = 0; i < removed_spheres.size(); i++)
			{
				m_sphere_list.instances.erase(m_sphere_list.instances.begin() + removed_spheres.top());
				removed_spheres.pop();
			}


			// update rigid body and render
			if (m_sphere_list.instances.size())
			{
				sphere_shader_program->bind();
				sphere_shader_program->set_mat4("View", view);
				sphere_shader_program->set_mat4("Projection", projection);
				m_sphere_list.render(sphere_shader_program);
			}
			///////////////////////////

			// directional light direction calculation
			// rotate directional light
			m_dirLigh.direction = glm::vec3(
				glm::rotate(glm::mat4(1.0f), glm::radians(0.5f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::vec4(m_dirLigh.direction, 1.0f)
			);

			// render directional light (send dirlight uniforms to shader_program)
			m_dirLigh.render(shader_program);
			m_dirLigh.render(sphere_shader_program);

			//setup spot light position and direction
			m_spotLight.position = m_player->get_camera()->get_position();
			m_spotLight.direction = m_player->get_camera()->get_cameraFront();
			m_spotLight.render(0, shader_program);
			if (mouse::button_pressed(GLFW_MOUSE_BUTTON_1))
			{
				shader_program->set_int("u_NSpotLights", 1);
				sphere_shader_program->set_int("u_NSpotLights", 1);
			}
			else
			{
				shader_program->set_int("u_NSpotLights", 0);
				sphere_shader_program->set_int("u_NSpotLights", 0);
			}

			// render point lights (send pointlights uniforms to shader_program)
			for (size_t i = 0; i < 4; i++)
			{
				m_lamps[i].pointlight.render(i, shader_program);
				m_lamps[i].pointlight.render(i, sphere_shader_program);
			}
			shader_program->set_int("u_NPointLights", 4);
			sphere_shader_program->set_int("u_NPointLights", 4);

			// render lamps
			Lmap_shader_program->bind();
			Lmap_shader_program->set_mat4("Projection", projection);
			Lmap_shader_program->set_mat4("View", view);

			for (size_t i = 0; i < 4; i++)
			{
				m_lamps[i].render(Lmap_shader_program);
			}
		}
	}
}

void Application::stop()
{
	m_running = false;
}

bool Application::init()
{
	// inialize All Engine Subsystems
	Log::init();
	m_window = std::make_shared<window>(m_title,m_width, m_height);
	if (m_window == nullptr)
	{
		KRAZY_ERROR("Application Failed to creat window object!");
		return false;
	}


	//TODO: Remove
	m_player = std::make_shared<fps_player>(glm::vec3(0.0f));

	Lmap_shader_program = std::make_shared<shader>("assets/shaders/model_vertex.glsl", "assets/shaders/lamp_fragment.glsl");
	shader_program = std::make_shared<shader>("assets/shaders/model_vertex.glsl", "assets/shaders/model_fragment.glsl");
	sphere_shader_program = std::make_shared<shader>("assets/shaders/model_instanced_vertex.glsl", "assets/shaders/model_fragment.glsl");
	// creat renderer
	//renderer::init(shader_program);
	//renderer::add_model(glm::vec3(0.0f), glm::vec3(0.25), "assets/models/player/scene.gltf");
	m_sphere_list.init();
	//m_sphere_list.set_scale(glm::vec3(0.25f));


	for (size_t i = 0; i < 4; i++)
	{
		m_lamps[i] = lamp(
			glm::vec3(1.0f),
			m_pointLightPositions[i],
			glm::vec3(0.25f),
			glm::vec4(0.05f,0.05f,0.05f,1.0f),
			glm::vec4(0.8f,0.8f,0.8f,1.0f),
			glm::vec4(1.0f),
			1.0f, 0.07f, 0.32f);

		m_lamps[i].init();
	}


	m_spotLight =
	{
		m_player->get_camera()->get_position(),
		m_player->get_camera()->get_cameraFront(),
		glm::cos(glm::radians(12.5f)),
		glm::cos(glm::radians(20.0f)),
		1.0f, 0.07f, 0.32f,
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
		glm::vec4(1.0f),
		glm::vec4(1.0f)
	};



	// app is initialized
	KRAZY_INFO("{} Application is initialized :)", m_title);

	m_instance = this;
	return true;
}

void Application::shutdown()
{
	m_running = false;
}

void Application::throw_sphere()
{
	rigid_body _rbody(1.0f, m_player->get_camera()->get_position());

	_rbody.add_impulse(m_player->get_camera()->get_cameraFront(), 3000.0f);
	_rbody.add_acceleration(physics_world::gravity_vector);

	m_sphere_list.instances.push_back(_rbody);
}