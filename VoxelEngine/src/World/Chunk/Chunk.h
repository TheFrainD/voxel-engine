#pragma once

// Headers
#include <Config.h>
#include <World/Block.h>
#include <World/Chunk/ChunkMesh.h>

#include <memory>
#include <array>

#include <glm/glm.hpp>

namespace Voxel
{

	class Chunk
	{
	public:
		static constexpr glm::ivec3 size = glm::ivec3(16, 128, 16);
		static constexpr Uint32 volume = size.x * size.y * size.z;

		explicit Chunk(const glm::ivec3& position);

		void GenerateMesh();
		const std::shared_ptr<ChunkMesh>& GetMesh() const { return mesh; }

		const std::shared_ptr<Block>& GetBlock(Int32 x, Int32 y, Int32 z);
		void SetBlock(Int32 x, Int32 y, Int32 z, const std::shared_ptr<Block>& block);

		void SetNeighbour(int side, const std::shared_ptr<Chunk>& chunk);

		void SetModified() { modified = true; }

		glm::ivec3 GetPosition() const { return position; }

		static std::shared_ptr<Chunk> Create(const glm::ivec3& position);
	private:
		bool modified;
		glm::ivec3 position;
		std::shared_ptr<ChunkMesh> mesh;
		std::array<std::shared_ptr<Block>, volume> blocks;
		std::array<std::shared_ptr<Chunk>, 6> neighbouring;
	};

}