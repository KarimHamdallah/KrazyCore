#include "texture.h"
#include "subsystems/asserts.h"
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include "utils/stb_image.h"

texture::texture(const std::string& dir, const std::string& path, aiTextureType type, bool flip, Filter min_filter, Filter mag_filter)
	: m_dir(dir),m_path(path), m_type(type)
{
	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter == Filter::linear ? GL_LINEAR : GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter == Filter::linear ? GL_LINEAR : GL_NEAREST);

	stbi_set_flip_vertically_on_load(flip);
	u8* data = stbi_load((dir + "/" + path).c_str(), &m_width, &m_height, &m_n_channel, 0);

	GLenum format = GL_RGB;
	GLenum internal_format = GL_RGB;

	if(m_n_channel == 4)
	{
		format = GL_RGBA;
		internal_format = GL_RGBA;
	}

	if(data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, internal_format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		KRAZY_ERROR("Failed to load texture at location : {}", (dir + "/" + path));
	}

	stbi_image_free(data);
}

texture::~texture()
{
	delete_id();
}

void texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, m_id);
}

void texture::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void texture::delete_id()
{
	glDeleteTextures(1, &m_id);
}

void texture::bind_slot(u32 slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
}