#include <vepch.h>
#include "World.h"

// Headers
#include <Config.h>
#include <Core/Input.h>
#include <World/Block.h>
#include <World/Chunk/Chunk.h>
#include <World/Chunk/ChunkMesh.h>
#include <World/Chunk/ChunkRenderer.h>
#include <World/WorldGenerator/WorldGenerator.h>
#include <World/WorldGenerator/FlatWorldGenerator.h>
#include <World/WorldGenerator/ClassicWorldGenerator.h>
#include <World/Noise/Random.h>

#include <memory>
#include <vector>
#include <cmath>
#include <climits>

#include <glm/glm.hpp>


namespace Voxel
{

	std::vector<std::shared_ptr<Chunk>> World::_chunks;
	std::shared_ptr<Camera3D> World::_camera;
	WorldGenerator* World::_worldGenerator;

	void World::Init(const Game* game)
	{
		_camera = Camera3D::Create(game, 60.0f, 0.02f);
		_camera->SetPosition({ 0.0f, 70.0f, 0.0f });
		ChunkRenderer::SetCurrentCamera(_camera);
		ChunkRenderer::Init();

		Block::LoadData();

		Random::Init();
		_worldGenerator = new ClassicWorldGenerator(volume);

		//Int8 counter = 1;
		//for (int cy = 0; cy < World::size.y; cy++)
		//{
		//	for (int cz = 0; cz < World::size.z; cz++)
		//	{
		//		for (int cx = 0; cx < World::size.x; cx++)
		//		{
		//			std::shared_ptr<Chunk> chunk = Chunk::Create(glm::ivec3(cx, cy, cz));

		//			for (int y = 0; y < Chunk::size.y; y++)
		//			{
		//				for (int z = 0; z < Chunk::size.z; z++)
		//				{
		//					for (int x = 0; x < Chunk::size.x; x++)
		//					{
		//						int sampleX = cx * Chunk::size.x + x;
		//						
		//						if (y <= (sinf(sampleX / 10.0f) * 25.0f) + 25)
		//						{
		//							if (y >= 30)
		//							{
		//								chunk->SetBlock(x, y, z, BlockType::Dirt);
		//							}
		//							else
		//							{
		//								chunk->SetBlock(x, y, z, BlockType::Stone);
		//							}
		//						}
		//						else
		//						{
		//							chunk->SetBlock(x, y, z, BlockType::Air);
		//						}
		//					}
		//				}
		//			}

		//			for (int y = 0; y < Chunk::size.y - 1; y++)
		//			{
		//				for (int z = 0; z < Chunk::size.z; z++)
		//				{
		//					for (int x = 0; x < Chunk::size.x; x++)
		//					{
		//						if (chunk->GetBlock(x, y, z)->id == BlockType::Dirt &&
		//							chunk->GetBlock(x, y + 1, z)->id == BlockType::Air)
		//						{
		//							chunk->SetBlock(x, y + 1, z, BlockType::Grass);
		//						}
		//					}
		//				}
		//			}

		//			_chunks[(cz * size.x * size.y) + (cy * size.x) + cx] = chunk;
		//			//_chunks.push_back(chunk);
		//			VE_LOG_INFO("Generating chunk {}/{}", counter++, volume);
		//		}
		//	}
		//}

		_worldGenerator->Generate(_chunks);

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

		VE_LOG_INFO("Generation finished");

	}

	void World::Regenerate(int seed) {
		_chunks.clear();

		_worldGenerator->Generate(_chunks, seed);

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
	
	}

	void World::Render()
	{
		ChunkRenderer::Prepare();
		
		for (auto chunk : _chunks)
		{
			chunk->GenerateMesh();
			ChunkRenderer::Render(chunk->GetMesh());
		}

		ChunkRenderer::End();
	}

	void World::Destroy()
	{
		delete _worldGenerator;
	}

	std::shared_ptr<Chunk> World::GetChunk(Int32 x, Int32 y, Int32 z)
	{
		if (x >= 0 && y >= 0 && z >= 0 && x < size.x && y < size.y && z < size.z) {
			return _chunks[(z * size.x * size.y) + (y * size.x) + x];
		}

		return nullptr;
	}

	Block* World::GetBlock(Int32 x, Int32 y, Int32 z)
	{
		int cx = x / Chunk::size.x;
		int cy = y / Chunk::size.y;
		int cz = z / Chunk::size.z;
		if (x < 0) cx--;
		if (y < 0) cy--;
		if (z > 0) cz--;
		if (cx < 0 || cy < 0 || cz < 0 || cx >= World::size.x || cy >= World::size.y || cz >= World::size.z)
			return nullptr;
		auto chunk = GetChunk(cx, cy, cz);
		int lx = x - Chunk::size.x * cx;
		int ly = y - Chunk::size.y * cy;
		int lz = z - Chunk::size.z * cz;
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
		int lx = x - Chunk::size.x * cx;
		int ly = y - Chunk::size.y * cy;
		int lz = z - Chunk::size.z * cz;
		chunk->SetBlock(lx, ly, lz, id);
	}

	Block* World::RayCast(glm::vec3 a, glm::vec3 dir, float maxDist, glm::vec3& end, glm::vec3& norm, glm::vec3& iend)
	{
		float px = a.x;
		float py = a.y;
		float pz = a.z;

		float dx = dir.x;
		float dy = dir.y;
		float dz = dir.z;

		float t = 0.0f;
		int ix = floor(px);
		int iy = floor(py);
		int iz = floor(pz);

		float stepx = (dx > 0.0f) ? 1.0f : -1.0f;
		float stepy = (dy > 0.0f) ? 1.0f : -1.0f;
		float stepz = (dz > 0.0f) ? 1.0f : -1.0f;

		float infinity = std::numeric_limits<float>::infinity();

		float txDelta = (dx == 0.0f) ? infinity : abs(1.0f / dx);
		float tyDelta = (dy == 0.0f) ? infinity : abs(1.0f / dy);
		float tzDelta = (dz == 0.0f) ? infinity : abs(1.0f / dz);

		float xdist = (stepx > 0) ? (ix + 1 - px) : (px - ix);
		float ydist = (stepy > 0) ? (iy + 1 - py) : (py - iy);
		float zdist = (stepz > 0) ? (iz + 1 - pz) : (pz - iz);

		float txMax = (txDelta < infinity) ? txDelta * xdist : infinity;
		float tyMax = (tyDelta < infinity) ? tyDelta * ydist : infinity;
		float tzMax = (tzDelta < infinity) ? tzDelta * zdist : infinity;

		int steppedIndex = -1;

		while (t <= maxDist) {
			Block* block = World::GetBlock(ix, iy, iz);
			if (block == nullptr || block->id) {
				end.x = px + t * dx;
				end.y = py + t * dy;
				end.z = pz + t * dz;

				iend.x = ix;
				iend.y = iy;
				iend.z = iz;

				norm.x = norm.y = norm.z = 0.0f;
				if (steppedIndex == 0) norm.x = -stepx;
				if (steppedIndex == 1) norm.y = -stepy;
				if (steppedIndex == 2) norm.z = -stepz;
				return block;
			}
			if (txMax < tyMax) {
				if (txMax < tzMax) {
					ix += stepx;
					t = txMax;
					txMax += txDelta;
					steppedIndex = 0;
				}
				else {
					iz += stepz;
					t = tzMax;
					tzMax += tzDelta;
					steppedIndex = 2;
				}
			}
			else {
				if (tyMax < tzMax) {
					iy += stepy;
					t = tyMax;
					tyMax += tyDelta;
					steppedIndex = 1;
				}
				else {
					iz += stepz;
					t = tzMax;
					tzMax += tzDelta;
					steppedIndex = 2;
				}
			}
		}
		iend.x = ix;
		iend.y = iy;
		iend.z = iz;

		end.x = px + t * dx;
		end.y = py + t * dy;
		end.z = pz + t * dz;
		norm.x = norm.y = norm.z = 0.0f;
		return nullptr;
	}

	const std::shared_ptr<Camera3D>& World::GetCamera()
	{
		return _camera;
	}

} // namespace Voxel