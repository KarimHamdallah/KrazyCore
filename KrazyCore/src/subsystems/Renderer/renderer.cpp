#include "renderer.h"
#include <core/Application.h>

std::vector<std::shared_ptr<model>> renderer::m_models;
std::shared_ptr<shader> renderer::m_shader_program;

void renderer::init(std::shared_ptr<shader>& shader_program)
{
	m_shader_program = shader_program;
}

void renderer::add_model(const glm::vec3 & position, const glm::vec3 & scale, const std::string & filepath, bool no_textures, bool flip_textures)
{
	std::shared_ptr<model> m = std::make_shared<model>(position, scale, no_textures, flip_textures);
	m->load(filepath);
	m_models.emplace_back(m);
}

void renderer::render_scene()
{
	// view and projection matrices
	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);

	f32 width = Application::get_Instance()->get_width();
	f32 height = Application::get_Instance()->get_height();

	f32 aspect_ratio = (f32)width / (f32)height;

	projection = glm::perspective(glm::radians(Application::get_Instance()->m_player->get_camera()->get_fov()), aspect_ratio, 0.1f, 1000.0f);
	view = Application::get_Instance()->m_player->get_camera()->get_view_matrix();

	m_shader_program->bind();
	m_shader_program->set_mat4("Projection", projection);
	m_shader_program->set_mat4("View", view);
	m_shader_program->set_vec3("u_ViewPos", Application::get_Instance()->m_player->get_camera()->get_position());

	for (auto& m : m_models)
	{
		// render mdoel
		m->render(m_shader_program);
	}
}