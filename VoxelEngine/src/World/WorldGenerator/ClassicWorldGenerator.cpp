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

	void ClassicWorldGenerator::Generate(std::vector<std::shared_ptr<Chunk>>& chunks, int seed) {

	}

	void ClassicWorldGenerator::Generate(std::vector<std::shared_ptr<Chunk>>& chunks, OctaveNoise *continentalnessNoise_, OctaveNoise *erosionNoise_, OctaveNoise *peaksNoise_)
	{
		continentalnessNoise = continentalnessNoise_;
		erosionNoise = erosionNoise_;
		peaksNoise = peaksNoise_;
		
		const Uint32 width = World::size.x * Chunk::size.x;
		const Uint32 depth = World::size.z * Chunk::size.z;
		std::vector<float> heightMap(width * depth, 0.0f);
		GenerateHeightmap(heightMap, width, depth);

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
	}

	void ClassicWorldGenerator::GenerateHeightmap(std::vector<float> &heightMap, Uint32 width, Uint32 depth)
	{	
		std::vector<double> continentalnessX = {0.0, 0.3, 0.45, 0.5, 0.6, 0.63, 0.8, 1.0};
   	std::vector<double> continentalnessY = {0.0, 0.2, 0.4, 0.4, 0.65, 0.68, 0.69, 0.65};
		tk::spline continentalnessSpline(continentalnessX, continentalnessY);

		std::vector<double> erosionX = {0.0, 0.15, 0.4, 0.5, 0.6, 0.75, 0.85, 0.95, 1.0};
   		std::vector<double> erosionY = {0.8, 0.55, 0.45, 0.55, 0.18, 0.15, 0.35, 0.02};
		tk::spline erosionSpline(erosionX, erosionY);

		std::vector<double> peaksX = {0.125, 0.3, 0.49, 0.75, 0.85, 0.95};
   		std::vector<double> peaksY = {0.02, 0.09, 0.24, 0.81, 0.91, 0.89};
		tk::spline peaksSpline(peaksX, peaksY);
		
		for (Int32 x = 0; x < width; x++)
		{
			for (Int32 z = 0; z < depth; z++)
			{
				float continentalnessFactor = continentalnessNoise->Compute(x / 2.0f, z / 2.0f);
				float continentalness = continentalnessSpline(continentalnessFactor);
				continentalness = powf(continentalness * 1.2f, 1.5f) / powf(1.2f, 1.5f);
				// continentalness = 0.1f + continentalness * (0.7f - 0.1f);

				float erosionFactor = erosionNoise->Compute(x / 3.0f, z / 3.0f);
				float erosion = erosionSpline(erosionFactor);
				erosion = powf(erosion * 1.2f, 3.5f) / powf(1.2f, 3.5f);
				erosion = 0.3f + erosion * (0.8f - 0.3f);

				float peaksFactor = peaksNoise->Compute(x * 2.0f, z * 2.0f);
				float peaks = peaksSpline(peaksFactor);
				peaks = powf(peaks * 1.2f, 4.5f) / powf(1.2f, 4.5f);

				float height;

				if (continentalnessFactor <= 0.2f){
					if (continentalness / erosion > continentalness) {
						height = LERP(continentalness, continentalness / erosion, INVERSE_LERP(0.0f, 0.2f, continentalnessFactor));
					} else {
						height = LERP(continentalness / erosion, continentalness, INVERSE_LERP(0.0f, 0.2f, continentalnessFactor));
					}
				} else {
					height = continentalness / erosion;
					if (erosionFactor <= 0.7f) {
						height = LERP(height * peaks, height, INVERSE_LERP(0.0f, 0.7f, erosionFactor));
					} else {
						height *= peaks;
					}
				}

				heightMap[x + z * width] = height;
			}
		}

		Normalize(heightMap, width, depth);
	}

} // namespace Voxel