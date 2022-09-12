#pragma once
#include "gl_base_elemnt.h"
#include <assimp/scene.h>
#include <string>

enum class Filter { linear, nearest };

class texture : public gl_base_element
{
public:
	texture() = default;
	texture(const std::string& dir,const std::string& path, aiTextureType type, bool flip = true, Filter min_filter = Filter::linear, Filter mag_filter = Filter::linear);
	~texture();

	virtual void bind() override;
	virtual void unbind() override;
	virtual void delete_id() override;

	void bind_slot(u32 slot = 0);

	inline i32 get_width() { return m_width; }
	inline i32 get_height() { return m_height; }

	inline std::string get_dir() const { return m_dir; }
	inline std::string get_path() const { return m_path; }
	inline aiTextureType get_type()const { return m_type; }
private:
	i32 m_width, m_height, m_n_channel;
	std::string m_dir, m_path;
	aiTextureType m_type;
};