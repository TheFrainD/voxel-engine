#include <vepch.h>
#include "Texture.h"

// Headers
#include <Config.h>
#include <Core/Assert.h>
#include <Core/Log.h>
#include <Utils/ResourceManager.h>

#include <string>
#include <memory>

#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Voxel
{

	Texture::Texture(const std::string& path)
	{
		int channels, width, height;
		Uint8* buffer = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
		if (buffer == nullptr)
		{
			VE_LOG_ERROR("Failed to load texture at \"{}\"", path);
			return;
		}
		
		this->width = width;
		this->height = height;

		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		// Configure texture
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Load image into the texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(buffer);

		Utils::ResourceManager::Add(this);
	}

	Texture::Texture(Uint32 width, Uint32 height)
	{
		this->width = width;
		this->height = height;

		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		// Configure texture
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glBindTexture(GL_TEXTURE_2D, 0);
		Utils::ResourceManager::Add(this);
	}

	void Texture::SetData(void* data, Uint32 size, bool alpha)
	{
		VE_ASSERT(size == width * height * 4, "Texture data must fill entire texture!");

		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, alpha ? GL_RGBA : GL_RGB, width, height, 0, alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::Destroy()
	{
		glDeleteTextures(1, &id);
	}

	void Texture::Bind(int unit) const
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, id);
	}

	void Texture::Bind() const
	{
		Bind(0);
	}

	std::shared_ptr<Texture> Texture::Create(const std::string& path)
	{
		return std::make_shared<Texture>(path);
	}

	std::shared_ptr<Texture> Texture::Create(Uint32 width, Uint32 height)
	{
		return std::make_shared<Texture>(width, height);
	}

} // namespace Voxel