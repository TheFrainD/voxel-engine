#pragma once

// Headers
#include <Config.h>
#include <World/WorldGenerator/WorldGenerator.h>
#include <World/Noise/OctaveNoise.h>

namespace Voxel
{

	class ClassicWorldGenerator : public WorldGenerator
	{
	public:
		ClassicWorldGenerator(Uint32 size) : WorldGenerator(size)
		{

		}

		~ClassicWorldGenerator() {
			if (continentalnessNoise) {
				delete continentalnessNoise;
			}

			if (peaksNoise) {
				delete peaksNoise;
			}

			if (erosionNoise) {
				delete erosionNoise;
			}
		}

		void Generate(std::vector<std::shared_ptr<Chunk>>& chunks, int seed = -1) override;
		void Generate(std::vector<std::shared_ptr<Chunk>>& chunks, OctaveNoise *continentalnessNoise_, OctaveNoise *erosionNoise_, OctaveNoise *peaksNoise_);

		OctaveNoise *GetContinentalnessNoise() { return continentalnessNoise; }
		OctaveNoise *GetErosionNoise() { return erosionNoise; }
		OctaveNoise *GetPeaksNoise() { return peaksNoise; }
	private:
		void GenerateHeightmap(std::vector<float> &heightMap, Uint32 width, Uint32 depth);

		Uint32 waterLevel = 32;
		OctaveNoise *continentalnessNoise;
		OctaveNoise *erosionNoise;
		OctaveNoise *peaksNoise;
	};

} // namespace Voxel