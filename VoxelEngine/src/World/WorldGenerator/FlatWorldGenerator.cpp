#include <vepch.h>
#include "FlatWorldGenerator.h"

// Headers
#include <Config.h>
#include <World/World.h>

namespace Voxel
{

	void FlatWorldGenerator::Generate(std::vector<std::shared_ptr<Chunk>>& chunks, int seed)
	{
		for (int cy = 0; cy < World::size.y; cy++)
		{
			for (int cz = 0; cz < World::size.z; cz++)
			{
				for (int cx = 0; cx < World::size.x; cx++)
				{
					std::shared_ptr<Chunk> chunk = Chunk::Create(glm::ivec3(cx, cy, cz));

					// Set ground
					for (int y = 0; y < Chunk::size.y; y++)
					{
						for (int z = 0; z < Chunk::size.z; z++)
						{
							for (int x = 0; x < Chunk::size.x; x++)
							{
								if (y <= 50)
								{
									if (y >= 30)
									{
										chunk->SetBlock(x, y, z, BlockType::Dirt);
									}
									else
									{
										chunk->SetBlock(x, y, z, BlockType::Stone);
									}
								}
								else
								{
									chunk->SetBlock(x, y, z, BlockType::Air);
								}
							}
						}
					}

					// Populate with grass
					for (int y = 0; y < Chunk::size.y - 1; y++)
					{
						for (int z = 0; z < Chunk::size.z; z++)
						{
							for (int x = 0; x < Chunk::size.x; x++)
							{
								if (chunk->GetBlock(x, y, z)->id == BlockType::Dirt &&
									chunk->GetBlock(x, y + 1, z)->id == BlockType::Air)
								{
									chunk->SetBlock(x, y + 1, z, BlockType::Water);
								}
							}
						}
					}

					chunks.push_back(chunk);
				}
			}
		}
	}

} // namespace Voxel