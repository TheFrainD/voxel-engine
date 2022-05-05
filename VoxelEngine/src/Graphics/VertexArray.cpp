#include <vepch.h>
#include "VertexArray.h"

// Headers
#include <Config.h>
#include <Core/Assert.h>
#include <Utils/ResourceManager.h>
#include <Graphics/Buffer.h>

#include <vector>
#include <memory>

#include <glad/glad.h>

namespace Voxel
{

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:    return GL_FLOAT;
		case ShaderDataType::Float2:   return GL_FLOAT;
		case ShaderDataType::Float3:   return GL_FLOAT;
		case ShaderDataType::Float4:   return GL_FLOAT;
		case ShaderDataType::Mat3:     return GL_FLOAT;
		case ShaderDataType::Mat4:     return GL_FLOAT;
		case ShaderDataType::Int:      return GL_INT;
		case ShaderDataType::Int2:     return GL_INT;
		case ShaderDataType::Int3:     return GL_INT;
		case ShaderDataType::Int4:     return GL_INT;
		case ShaderDataType::Bool:     return GL_BOOL;
		}

		VE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &varray);
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(varray);
	}

	void VertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		VE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		glBindVertexArray(varray);
		vertexBuffer->Bind();

		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			switch (element.type)
			{
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
			{
				glEnableVertexAttribArray(vertexBufferIndex);
				glVertexAttribPointer(vertexBufferIndex,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.type),
					element.normalized ? GL_TRUE : GL_FALSE,
					layout.GetStride(),
					(const void*)element.offset);
				vertexBufferIndex++;
				break;
			}
			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
			case ShaderDataType::Bool:
			{
				glEnableVertexAttribArray(vertexBufferIndex);
				glVertexAttribIPointer(vertexBufferIndex,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.type),
					layout.GetStride(),
					(const void*)element.offset);
				vertexBufferIndex++;
				break;
			}
			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4:
			{
				uint8_t count = element.GetComponentCount();
				for (uint8_t i = 0; i < count; i++)
				{
					glEnableVertexAttribArray(vertexBufferIndex);
					glVertexAttribPointer(vertexBufferIndex,
						count,
						ShaderDataTypeToOpenGLBaseType(element.type),
						element.normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)(element.offset + sizeof(float) * count * i));
					glVertexAttribDivisor(vertexBufferIndex, 1);
					vertexBufferIndex++;
				}
				break;
			}
			default:
				VE_ASSERT(false, "Unknown ShaderDataType!");
			}
		}

		vertexBuffers.push_back(vertexBuffer);
	}

	void VertexArray::SetElementBuffer(const std::shared_ptr<ElementBuffer>& elementBuffer)
	{
		glBindVertexArray(varray);
		elementBuffer->Bind();
		this->elementBuffer = elementBuffer;
	}

	void VertexArray::Destroy()
	{
		glDeleteVertexArrays(1, &varray);
	}

	std::shared_ptr<VertexArray> VertexArray::Create()
	{
		return std::make_shared<VertexArray>();
	}

} // namespace Voxel