#pragma once
#include <glad/glad.h>
#include "model.h"
#include "defines.h"


class render_command
{
public:
	inline static void draw_indexed(u32 count) { glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr); }
	inline static void draw_indexed(std::shared_ptr<vertex_array>& vao) { vao->bind(); glDrawElements(GL_TRIANGLES, vao->get_index_buffer()->get_count(), GL_UNSIGNED_INT, nullptr); }
	inline static void resize_viewport(u32 width, u32 height) { glViewport(0, 0, width, height); }
	inline static void clear_buffer(GLbitfield flags = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) { glClear(flags); }
	inline static void clear_color(f32 r, f32 g, f32 b, f32 a = 1.0f) { glClearColor(RGB_COLOR(r, g, b)); }
private:
};

class renderer
{
public:
	static void init(std::shared_ptr<shader>& shader_program);
	static void add_model(const glm::vec3& position, const glm::vec3& scale,
		const std::string& filepath, bool no_textures = false, bool flip_textures = false);

	static void render_scene();

	inline static std::shared_ptr<shader> get_shader() { return m_shader_program; }
	
	static std::vector<std::shared_ptr<model>> m_models;
private:
	static std::shared_ptr<shader> m_shader_program;
};