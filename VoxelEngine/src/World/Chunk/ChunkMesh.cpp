#include <vepch.h>
#include "ChunkMesh.h"

// Headers
#include <Config.h>
#include <Graphics/VertexArray.h>
#include <Graphics/Buffer.h>

#include <vector>
#include <memory>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace Voxel
{

	ChunkMesh::ChunkMesh(Uint32 volume) :
		position(glm::vec3(0.0f))
	{
		vertexArray = VertexArray::Create();

		vertexBuffer = VertexBuffer::Create(volume * sizeof(Vertex) * 24);
		vertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_position"	},
			{ ShaderDataType::Float2, "a_uv"		},
			{ ShaderDataType::Float,  "a_light"		}
		});
		vertexArray->AddVertexBuffer(vertexBuffer);

		elementBuffer = ElementBuffer::Create(volume * sizeof(Vertex) * 36);
		vertexArray->SetElementBuffer(elementBuffer);
	}

	void ChunkMesh::SetData(const std::vector<Vertex>& vertices, const std::vector<Uint32>& elements)
	{
		if (vertices.size() != 0)
		{
			vertexBuffer->SetData(&vertices[0], sizeof(Vertex) * vertices.size());
		}
		if (elements.size() != 0)
		{
			elementBuffer->SetData(elements);
		}
		
		size = elements.size();
	}

	void ChunkMesh::Bind() const
	{
		vertexArray->Bind();
		elementBuffer->Bind();
	}

	glm::mat4 ChunkMesh::GetModel() const
	{
		return glm::translate(glm::mat4(1.0f), position);
	}

	std::shared_ptr<ChunkMesh> ChunkMesh::Create(Uint32 volume)
	{
		return std::make_shared<ChunkMesh>(volume);
	}

} // namespace Voxel