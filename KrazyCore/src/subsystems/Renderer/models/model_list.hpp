#pragma once

#include <defines.h>
#include <glm/glm.hpp>
#include <subsystems/physics/rigid_body.h>
#include <subsystems/Renderer/shader.h>
#include <memory>
#include <vector>

template <typename T>
class model_list
{
public:
	void init()
	{
		m_model.init();
		// creat positions buffer
		glCreateBuffers(1, &positions_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, positions_vbo);
		glBufferData(GL_ARRAY_BUFFER, max_instances * sizeof(glm::vec3), nullptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// creat scales buffer
		glCreateBuffers(1, &scales_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, scales_vbo);
		glBufferData(GL_ARRAY_BUFFER, max_instances * sizeof(glm::vec3), nullptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		for (int i = 0, size = m_model.meshes.size(); i < size; i++)
		{
			glBindVertexArray(m_model.meshes[i]->get_vao()->m_id);
			glBindBuffer(GL_ARRAY_BUFFER, positions_vbo);

			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
			glad_glEnableVertexAttribArray(3);

			glBindBuffer(GL_ARRAY_BUFFER, scales_vbo);
			
			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
			glad_glEnableVertexAttribArray(4);

			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glVertexAttribDivisor(3, 1);
			glVertexAttribDivisor(4, 1);

			glBindVertexArray(0);
		}
	}

	void render(const std::shared_ptr<shader>& shader_program, bool reset_instances = true)
	{
		if (reset_instances)
		{
			positions.clear();
			scales.clear();

			for (rigid_body& rbody : instances)
			{
				rbody.update();
				positions.push_back(rbody.rb_pos);
				scales.push_back(m_model.scale);
			}
		}

		shader_program->set_mat4("Model", glm::mat4(1.0f));

		m_model.render(shader_program, false, false);

		int size = std::min((int)max_instances, (int)positions.size());

		if (size != 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, positions_vbo);
			glBufferSubData(GL_ARRAY_BUFFER, 0, size * sizeof(glm::vec3), &positions[0]);

			glBindBuffer(GL_ARRAY_BUFFER, scales_vbo);
			glBufferSubData(GL_ARRAY_BUFFER, 0, size * sizeof(glm::vec3), &scales[0]);
		}

		for (int i = 0, mesh_count = m_model.meshes.size(); i < mesh_count; i++)
		{
			glBindVertexArray(m_model.meshes[i]->get_vao()->m_id);
			glDrawElementsInstanced(GL_TRIANGLES, m_model.meshes[i]->indices.size(), GL_UNSIGNED_INT, nullptr, size);
		}
	}

	void set_scale(const glm::vec3& scale)
	{
		m_model.scale = scale;
	}

	std::vector<rigid_body> instances;
protected:
	T m_model;

	u32 positions_vbo, scales_vbo;
	std::vector<glm::vec3> positions, scales;

	u32 max_instances = 200;
};