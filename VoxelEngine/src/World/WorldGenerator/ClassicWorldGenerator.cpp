#include <vepch.h>
#include "ClassicWorldGenerator.h"

// Headers
#include <Config.h>
#include <World/World.h>
#include <World/Noise/Random.h>

#include <spline.h>

namespace Voxel
{
	void Normalize(std::vector<float> &heightMap, Uint32 width, Uint32 depth) {
		float min = heightMap[0], max = heightMap[0];

		for (int i = 0; i < width * depth; ++i) {
			auto val = heightMap[i];
			if (val > max) {
				max = val;
			}
			if (val < min) {
				min = val;
			}
		}

		for (int i = 0; i < width * depth; ++i) {
			heightMap[i] = ((heightMap[i] - min) / (max - min)) * 100;
		}
	}

	void ClassicWorldGenerator::Generate(std::vector<std::shared_ptr<Chunk>>& chunks, int seed)
	{
		const Uint32 width = World::size.x * Chunk::size.x;
		const Uint32 depth = World::size.z * Chunk::size.z;
		Uint32 *heightMap = new Uint32[width * depth];
		GenerateHeightmap(heightMap, width, depth, seed);

		for (int cy = 0; cy < World::size.y; cy++)
		{
			for (int cz = 0; cz < World::size.z; cz++)
			{
				for (int cx = 0; cx < World::size.x; cx++)
				{
					std::shared_ptr<Chunk> chunk = Chunk::Create(glm::ivec3(cx, cy, cz));

					OctaveNoise dirtNoise(2, 0.2f, 2.0f);

					// Set ground
					for (int y = 0; y < Chunk::size.y; y++)
					{
						for (int z = 0; z < Chunk::size.z; z++)
						{
							for (int x = 0; x < Chunk::size.x; x++)
							{
								Uint32 sampleX = x + cx * Chunk::size.x;
								Uint32 sampleY = y + cy * Chunk::size.y;
								Uint32 sampleZ = z + cz * Chunk::size.z;

								Uint32 height = heightMap[sampleX + sampleZ * World::size.x * Chunk::size.x];

								float dirtBaseThickness = 4;
								float dirtThickness = LERP(1, dirtBaseThickness, dirtNoise.Compute(sampleX * 20.0f, sampleZ * 20.0f));
								float dirtMaxHeight = 50 + dirtThickness;
								float stoneTransition = height;
								float dirtTransition = stoneTransition - dirtThickness;

								Uint32 blockType = BlockType::Air;
								if (sampleY == 0)
								{
									blockType = BlockType::Bedrock;
								}
								else if (sampleY <= stoneTransition)
								{
									if (sampleY >= dirtTransition && sampleY <= dirtMaxHeight)
									{
										blockType = BlockType::Dirt;
									}
									else
									{
										blockType = BlockType::Stone;
									}
								}
								else if (sampleY <= waterLevel)
								{
									blockType = BlockType::Water;
								}

								chunk->SetBlock(x, y, z, blockType);
							}
						}
					}

					OctaveNoise sandNoise(2, 0.4f, 2.2f);

					// Populate with grass and sand
					for (int z = 0; z < Chunk::size.z; z++)
					{
						for (int x = 0; x < Chunk::size.x; x++)
						{
							Uint32 sampleX = x + cx * Chunk::size.x;
							Uint32 sampleZ = z + cz * Chunk::size.z;

							Uint32 height = heightMap[sampleX + sampleZ * World::size.x * Chunk::size.x];

							if (height <= waterLevel + 3)
							{
								if ((chunk->GetBlock(x, height + 1, z)->id == BlockType::Water || chunk->GetBlock(x, height + 1, z)->id == BlockType::Air)&&
									chunk->GetBlock(x, height, z)->id == BlockType::Dirt && height >= waterLevel - 15)
								{
									chunk->SetBlock(x, height, z, BlockType::Sand);
								}
							}
							else if (chunk->GetBlock(x, height + 1, z)->id == BlockType::Air && 
								chunk->GetBlock(x, height, z)->id == BlockType::Dirt)
							{
								chunk->SetBlock(x, height, z, BlockType::Grass);
							}
						}
					}

					chunks.push_back(chunk);
				}
			}
		}

		delete[] heightMap;
	}

	void ClassicWorldGenerator::GenerateHeightmap(Uint32* heightMap, Uint32 width, Uint32 depth, int seed)
	{
		if (continentalnessNoise) {
			delete continentalnessNoise;
		}

		if (peaksNoise) {
			delete peaksNoise;
		}

		if (erosionNoise) {
			delete erosionNoise;
		}

		if (seed == -1) {
			seed = Random::Next();
		}
		
		continentalnessNoise = new OctaveNoise(4, 0.25f, 4.0f, seed);
		erosionNoise = new OctaveNoise(7, 2.0f, 0.7f, seed * 2);
		peaksNoise = new OctaveNoise(8, 0.3f, 1.3f, seed * 3);
		
		for (Int32 x = 0; x < width; x++)
		{
			for (Int32 z = 0; z < depth; z++)
			{
				float height = 0.0f;
				
				float continentalness = continentalnessNoise->Compute(x, z);
				float continentalHeight = 0.0f;
				
				if (continentalness < 0.1)
				{
					continentalHeight = LERP(120, 20, INVERSE_LERP(0.0, 0.1, continentalness));
				}
				else if (continentalness < 0.3)
				{
					continentalHeight = LERP(20, 35, INVERSE_LERP(0.1, 0.3, continentalness));
				}
				else if (continentalness < 0.4)
				{
					continentalHeight = LERP(35, 50, INVERSE_LERP(0.3, 0.4, continentalness));
				}
				else if (continentalness < 0.5)
				{
					continentalHeight = LERP(50, 55, INVERSE_LERP(0.4, 0.5, continentalness));
				}
				else if (continentalness < 0.65)
				{
					continentalHeight = LERP(50, 90, INVERSE_LERP(0.5, 0.65, continentalness));
				}
				else if (continentalness < 0.75)
				{
					continentalHeight = LERP(90, 100, INVERSE_LERP(0.65, 0.75, continentalness));
				}
				else
				{
					continentalHeight = LERP(100, 120, INVERSE_LERP(0.75, 1.0, continentalness));
				}

				height += continentalHeight / 3.0f;


				float erosion = erosionNoise->Compute(x, z);
				float erosionHeight = 0.0f;

				if (erosion < 0.2)
				{
					erosionHeight = LERP(120, 80, INVERSE_LERP(0.0, 0.2, erosion));
				}
				else if (erosion < 0.4)
				{
					erosionHeight = LERP(80, 50, INVERSE_LERP(0.2, 0.4, erosion));
				}
				else if (erosion < 0.5)
				{
					erosionHeight = LERP(50, 20, INVERSE_LERP(0.4, 0.5, erosion));
				}
				else if (erosion < 0.7)
				{
					erosionHeight = LERP(20, 10, INVERSE_LERP(0.5, 0.7, erosion));
				}
				else
				{
					erosionHeight = LERP(10, 5, INVERSE_LERP(0.7, 1.0, erosion));
				}

				height += erosionHeight / 3.0f;


				float peaks = peaksNoise->Compute(x, z);
				float peakHeight = 0.0f;

				if (peaks < 0.3)
				{
					peakHeight = LERP(5, 10, INVERSE_LERP(0.0, 0.3, peaks));
				}
				else if (peaks < 0.5)
				{
					peakHeight = LERP(10, 25, INVERSE_LERP(0.3, 0.5, peaks));
				}
				else if (peaks < 0.7)
				{
					peakHeight = LERP(25, 75, INVERSE_LERP(0.5, 0.7, peaks));
				}
				else if (peaks < 0.8)
				{
					peakHeight = LERP(75, 90, INVERSE_LERP(0.7, 0.8, peaks));
				}
				else
				{
					peakHeight = LERP(90, 120, INVERSE_LERP(0.8, 1.0, peaks));
				}

				height += peakHeight / 3.0f;

				heightMap[x + z * width] = height;
			}
		}
	}

} // namespace Voxel