#include <vepch.h>
#include "Buffer.h"

// Headers
#include <Config.h>
#include <Utils/ResourceManager.h>

#include <memory>

#include <glad/glad.h>

namespace Voxel
{
	
	VertexBuffer::VertexBuffer(Uint32 size)
	{
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);

		Utils::ResourceManager::Add(this);
	}

	VertexBuffer::VertexBuffer(const float* data, Uint32 size)
	{
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

		Utils::ResourceManager::Add(this);
	}

	void VertexBuffer::SetData(const void* data, Uint32 size)
	{
		if (size == 0)
			return;
		
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
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

	std::shared_ptr<VertexBuffer> VertexBuffer::Create(Uint32 size)
	{
		return std::make_shared<VertexBuffer>(size);
	}

	std::shared_ptr<VertexBuffer> VertexBuffer::Create(const float* data, Uint32 size)
	{
		return std::make_shared<VertexBuffer>(data, size);
	}

	ElementBuffer::ElementBuffer(Uint32 count) :
		buffer(0), count(count)
	{
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), nullptr, GL_DYNAMIC_DRAW);

		Utils::ResourceManager::Add(this);
	}

	ElementBuffer::ElementBuffer(Uint32* elements, Uint32 count) :
		buffer(0), count(count)
	{
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), elements, GL_STATIC_DRAW);

		Utils::ResourceManager::Add(this);
	}

	void ElementBuffer::SetData(Uint32* elements, Uint32 count)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), elements, GL_STATIC_DRAW);
	}

	void ElementBuffer::SetData(const std::vector<Uint32>& elements)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(uint32_t), &elements[0], GL_STATIC_DRAW);
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

	std::shared_ptr<ElementBuffer> ElementBuffer::Create(Uint32 count)
	{
		return std::make_shared<ElementBuffer>(count);
	}

	std::shared_ptr<ElementBuffer> ElementBuffer::Create(Uint32* elements, Uint32 count)
	{
		return std::make_shared<ElementBuffer>(elements, count);
	}

} // namespace Voxel