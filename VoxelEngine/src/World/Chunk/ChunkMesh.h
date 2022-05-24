#pragma once

// Headers
#include <Config.h>
#include <Graphics/VertexArray.h>
#include <Graphics/Buffer.h>

#include <vector>
#include <memory>

#include <glm/glm.hpp>

namespace Voxel
{

	class ChunkMesh
	{
	public:
		struct Vertex
		{
			glm::vec3 position;
			glm::vec2 uv;

			Vertex(const glm::vec3& position,const glm::vec2& uv)
				: position(position), uv(uv)
			{

			}
		};

		std::vector<Vertex> vertices;
		std::vector<Uint32> elements;
		glm::vec3 position;

		ChunkMesh(Uint32 volume);

		void SetData();
		void Bind() const;

		glm::mat4 GetModel() const;

		static std::shared_ptr<ChunkMesh> Create(Uint32 volume);
	private:
		std::shared_ptr<VertexArray> vertexArray;
		std::shared_ptr<VertexBuffer> vertexBuffer;
		std::shared_ptr<ElementBuffer> elementBuffer;
	};

} // namespace Voxel