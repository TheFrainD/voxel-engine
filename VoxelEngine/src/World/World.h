#pragma once

// Headers
#include <Config.h>
#include <Core/Game.h>
#include <Graphics/Camera3D.h>
#include <World/Block.h>
#include <World/Chunk/Chunk.h>
#include <World/Chunk/ChunkMesh.h>
#include <World/WorldGenerator/WorldGenerator.h>

#include <memory>
#include <vector>

#include <glm/glm.hpp>

namespace Voxel
{

	class World
	{
	public:
		static constexpr glm::ivec3 size = glm::ivec3(8, 1, 8);
		static constexpr Uint32 volume = size.x * size.y * size.z;

		static void Init(const Game* game);
		static void Regenerate(int seed = -1);
		static void Update();
		static void Render();
		static void Destroy();

		static std::shared_ptr<Chunk> GetChunk(Int32 x, Int32 y, Int32 z);
		static Block* GetBlock(Int32 x, Int32 y, Int32 z);
		static void SetBlock(Int32 x, Int32 y, Int32 z, Uint32 id);
		static Block* RayCast(glm::vec3 a, glm::vec3 dir, float max_dist, glm::vec3& end, glm::vec3& norm, glm::vec3& iend);

		static const std::shared_ptr<Camera3D>& GetCamera();

		static WorldGenerator* GetWorldGenerator() { return _worldGenerator; }
	private:
		static std::vector<std::shared_ptr<Chunk>> _chunks;
		static std::shared_ptr<Camera3D> _camera;
		static WorldGenerator* _worldGenerator;
	};

} // namespace Voxel