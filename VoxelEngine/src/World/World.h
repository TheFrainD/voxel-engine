#pragma once

// Headers
#include <Config.h>
#include <Core/Game.h>
#include <Graphics/Camera3D.h>
#include <World/Block.h>
#include <World/Chunk/Chunk.h>
#include <World/Chunk/ChunkMesh.h>

#include <memory>
#include <array>

#include <glm/glm.hpp>

namespace Voxel
{

	class World
	{
	public:
		static constexpr glm::ivec3 size = glm::ivec3(4, 1, 4);
		static constexpr Uint32 volume = size.x * size.y * size.z;

		static void Init(const Game* game);
		static void Update();
		static void Render();
		static void Destroy();

		static std::shared_ptr<Chunk> GetChunk(Int32 x, Int32 y, Int32 z);
		static Block* GetBlock(Int32 x, Int32 y, Int32 z);
		static void SetBlock(Int32 x, Int32 y, Int32 z, Uint32 id);

		static const std::shared_ptr<Camera3D>& GetCamera();
	private:
		static std::array<std::shared_ptr<Chunk>, volume> _chunks;
		static std::shared_ptr<Camera3D> _camera;
	};

} // namespace Voxel