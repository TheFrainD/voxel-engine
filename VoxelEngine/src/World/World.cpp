#include <vepch.h>
#include "World.h"

// Headers
#include <Config.h>
#include <World/Block.h>
#include <World/Chunk/Chunk.h>
#include <World/Chunk/ChunkMesh.h>
#include <World/Chunk/ChunkRenderer.h>

#include <memory>
#include <array>

#include <glm/glm.hpp>

namespace Voxel
{

	std::array<std::shared_ptr<Chunk>, World::volume> World::_chunks;
	std::shared_ptr<Camera3D> World::_camera;

	void World::Init(const Game* game)
	{
		_camera = Camera3D::Create(game, 60.0f, 0.02f);
		_camera->SetPosition({ 0.0f, 70.0f, 0.0f });
		ChunkRenderer::SetCurrentCamera(_camera);
		ChunkRenderer::Init();

		Block::LoadData();

		for (int cy = 0; cy < World::size.y; cy++)
		{
			for (int cz = 0; cz < World::size.z; cz++)
			{
				for (int cx = 0; cx < World::size.x; cx++)
				{
					std::shared_ptr<Chunk> chunk = Chunk::Create(glm::ivec3(cx, cy, cz));

					for (int y = 0; y < Chunk::size.y; y++)
					{
						for (int z = 0; z < Chunk::size.z; z++)
						{
							for (int x = 0; x < Chunk::size.x; x++)
							{
								int sampleX = cx * Chunk::size.x + x;
								
								if (y <= fabs(sinf(sampleX / 5.0f) * 50.0f))
								{
									chunk->SetBlock(x, y, z, Block::Create(BlockType::Stone));
								}
								else
								{
									chunk->SetBlock(x, y, z, Block::Create(BlockType::Air));
								}
							}
						}
					}

					_chunks[(cz * size.x * size.y) + (cy * size.x) + cx] = chunk;
				}
			}
		}

		for (int cy = 0; cy < size.y; cy++)
		{
			for (int cz = 0; cz < size.z; cz++)
			{
				for (int cx = 0; cx < size.x; cx++)
				{
					std::shared_ptr<Chunk> chunk = GetChunk(cx, cy, cz);

					chunk->SetNeighbour(Side::Top, GetChunk(cx, cy + 1, cz));
					chunk->SetNeighbour(Side::Bottom, GetChunk(cx, cy - 1, cz));
					chunk->SetNeighbour(Side::Front, GetChunk(cx, cy, cz + 1));
					chunk->SetNeighbour(Side::Back, GetChunk(cx, cy, cz - 1));
					chunk->SetNeighbour(Side::Right, GetChunk(cx + 1, cy, cz));
					chunk->SetNeighbour(Side::Left, GetChunk(cx - 1, cy, cz));

					chunk->GenerateMesh();
				}
			}
		}
	}

	void World::Update()
	{	
		_camera->Update();
		
		for (auto chunk : _chunks)
		{
			chunk->GenerateMesh();
		}
	}

	void World::Render()
	{
		for (auto chunk : _chunks)
		{
			ChunkRenderer::Render(chunk->GetMesh());
		}
	}

	void World::Destroy()
	{
	}

	std::shared_ptr<Chunk> World::GetChunk(Int32 x, Int32 y, Int32 z)
	{
		if (x >= 0 && y >= 0 && z >= 0 && x < size.x && y < size.y && z < size.z) {
			return _chunks[(z * size.x * size.y) + (y * size.x) + x];
		}

		return nullptr;
	}

	std::shared_ptr<Block> World::GetBlock(Int32 x, Int32 y, Int32 z)
	{
		int cx = x / Chunk::size.x;
		int cy = y / Chunk::size.y;
		int cz = z / Chunk::size.z;
		if (x < 0) cx--;
		if (y < 0) cy--;
		if (z < 0) cz--;
		if (cx < 0 || cy < 0 || cz < 0 || cx >= World::size.x || cy >= World::size.y || cz >= World::size.z)
			return nullptr;
		auto chunk = GetChunk(cx, cy, cz);
		int lx = x - Chunk::size.x;
		int ly = y - Chunk::size.y;
		int lz = z - Chunk::size.z;
		return chunk->GetBlock(lx, ly, lz);
	}

	void World::SetBlock(Int32 x, Int32 y, Int32 z, Uint32 id)
	{
		int cx = x / Chunk::size.x;
		int cy = y / Chunk::size.y;
		int cz = z / Chunk::size.z;
		if (x < 0) cx--;
		if (y < 0) cy--;
		if (z < 0) cz--;
		if (cx < 0 || cy < 0 || cz < 0 || cx >= World::size.x || cy >= World::size.y || cz >= World::size.z)
			return;
		auto chunk = GetChunk(cx, cy, cz);
		int lx = x - Chunk::size.x;
		int ly = y - Chunk::size.y;
		int lz = z - Chunk::size.z;
		chunk->SetBlock(lx, ly, lz, Block::Create(id));
	}

	const std::shared_ptr<Camera3D>& World::GetCamera()
	{
		return _camera;
	}

} // namespace Voxel