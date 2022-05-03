#include <vepch.h>
#include "Buffer.h"

// Headers
#include <Config.h>
#include <Utils/ResourceManager.h>

#include <glad/glad.h>

namespace Voxel
{
	void VertexBuffer::Create(Uint32 size)
	{
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);

		Utils::ResourceManager::Add(this);
	}

	void VertexBuffer::Create(const float* data, Uint32 size)
	{
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

		Utils::ResourceManager::Add(this);
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
	}

	void VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::Destroy()
	{
		glDeleteBuffers(1, &buffer);
	}

	void ElementBuffer::Create(Uint32* elements, Uint32 count)
	{
		glCreateBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), elements, GL_STATIC_DRAW);

		Utils::ResourceManager::Add(this);
	}

	void ElementBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
	}

	void ElementBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void ElementBuffer::Destroy()
	{
		glDeleteBuffers(1, &buffer);
	}

} // namespace Voxel