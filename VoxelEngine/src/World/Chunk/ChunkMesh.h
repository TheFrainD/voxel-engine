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
			float light;

			Vertex() = default;

			Vertex(const glm::vec3& position, const glm::vec2& uv, float light)
				: position(position), uv(uv), light(light)
			{

			}
		};

		glm::vec3 position;

		ChunkMesh(Uint32 volume);

		void SetData(const std::vector<Vertex>& vertices, const std::vector<Uint32>& elements);

		void Bind() const;

		glm::mat4 GetModel() const;
		Uint32 GetSize() const { return size; }

		static std::shared_ptr<ChunkMesh> Create(Uint32 volume);
	private:
		Uint32 size = 0;

		std::shared_ptr<VertexArray> vertexArray;
		std::shared_ptr<VertexBuffer> vertexBuffer;
		std::shared_ptr<ElementBuffer> elementBuffer;
	};

} // namespace Voxel