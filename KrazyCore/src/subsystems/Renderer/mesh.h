#pragma once
#include <defines.h>
#include <subsystems/Renderer/vertex_array.h>
#include <subsystems/Renderer/texture.h>
#include <subsystems/Renderer/shader.h>
#include <glm/glm.hpp>
#include <vector>

struct vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 tex_coord;

	static std::vector<vertex> gnerate_vertices(f32* vertices, u32 count);
};


class mesh
{
public:
	mesh() = default;
	mesh(const std::vector<vertex>& vertices, const std::vector<u32>& indices, const std::vector<std::shared_ptr<texture>>& textures = {});
	mesh(const std::vector<vertex>& vertices, const std::vector<u32>& indices, const aiColor4D& diffuse, const aiColor4D& specular);

	void render(const std::shared_ptr<shader>& shader_program, bool do_draw_call = true);

	std::vector<vertex> vertices;
	std::vector<u32> indices;
	std::vector<std::shared_ptr<texture>> textures;

	inline std::shared_ptr<vertex_array> get_vao() const { return m_vao; }

	inline aiColor4D get_diffuse() { return m_diffuse; }
	inline aiColor4D get_specular() { return m_specular; }
private:
	std::shared_ptr<vertex_buffer> m_vbo;
	std::shared_ptr<index_buffer> m_ibo;
	std::shared_ptr<vertex_array> m_vao;

	aiColor4D m_diffuse, m_specular;
	bool m_no_textures;

	void init();
};