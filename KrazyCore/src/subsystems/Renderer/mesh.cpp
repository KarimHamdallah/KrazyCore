#include "mesh.h"
#include <subsystems/Renderer/renderer.h>
#include <assimp/scene.h>

mesh::mesh(const std::vector<vertex>& vertices, const std::vector<u32>& indices, const std::vector<std::shared_ptr<texture>>& textures)
	: vertices(vertices),indices(indices),textures(textures),m_no_textures(false)
{
	init();
}

mesh::mesh(const std::vector<vertex>& vertices, const std::vector<u32>& indices, const aiColor4D & diffuse, const aiColor4D & specular)
	: vertices(vertices),indices(indices),m_diffuse(diffuse),m_specular(specular),m_no_textures(true)
{
	init();
}

void mesh::render(const std::shared_ptr<shader>& shader_program, bool do_draw_call)
{
	//shader_program->bind();

	if (m_no_textures)
	{
		shader_program->set_vec4("u_Material.diffuse", m_diffuse);
		shader_program->set_vec4("u_Material.specular", m_specular);
		shader_program->set_int("u_NoTex", 1);
	}
	else
	{
		u32 diffuse_idx = 0;
		u32 specular_idx = 0;

		for (int i = 0; i < textures.size(); i++)
		{
			textures[i]->bind_slot(i);
			std::string texture_name;

			aiTextureType T = textures[i]->get_type();
			switch (textures[i]->get_type())
			{
			case aiTextureType_DIFFUSE:
			{
				texture_name = "diffuse" + std::to_string(diffuse_idx++);
			}
			break;
			case aiTextureType_SPECULAR:
			{
				texture_name = "specular" + std::to_string(specular_idx++);
			}
			break;
			default:
				break;
			}
			shader_program->set_int(texture_name, i);
			textures[i]->bind();
		}
	}

	if (do_draw_call)
	{
		render_command::draw_indexed(m_vao);
		m_vao->unbind();

		glActiveTexture(GL_TEXTURE0);
	}
}

void mesh::init()
{
	m_vao = std::make_shared<vertex_array>();
	m_vbo = std::make_shared<vertex_buffer>(&vertices[0], vertices.size() * sizeof(vertex));
	m_ibo = std::make_shared<index_buffer>(&indices[0], indices.size());

	m_vao->add_vertex_buffer(m_vbo);
	m_vao->set_index_buffer(m_ibo);

	m_vao->set_vertex_layout(0, 3, sizeof(vertex), (void*)offsetof(vertex, position));
	m_vao->set_vertex_layout(1, 3, sizeof(vertex), (void*)offsetof(vertex, normal));
	m_vao->set_vertex_layout(2, 2, sizeof(vertex), (void*)offsetof(vertex, tex_coord));

	m_vao->unbind();
}

std::vector<vertex> vertex::gnerate_vertices(f32 * vertices, u32 count)
{
	std::vector<vertex> ret(count);

	// number of floats per vertex
	u32 stride = sizeof(vertex) / sizeof(f32);

	for (size_t i = 0; i < count; i++)
	{
		ret[i].position = glm::vec3(vertices[i * stride + 0], vertices[i * stride + 1], vertices[i * stride + 2]);
		ret[i].normal = glm::vec3(vertices[i * stride + 3], vertices[i * stride + 4], vertices[i * stride + 5]);
		ret[i].tex_coord = glm::vec2(vertices[i * stride + 6], vertices[i * stride + 7]);
	}

	return ret;
}