#pragma once

// Headers
#include <Config.h>
#include <World/Noise/Noise.h>
#include <World/Noise/PerlinNoise.h>

namespace Voxel
{

	class OctaveNoise : public Noise
	{
	public:
		OctaveNoise(Int32 octaves, float persistance, float lacunarity, int seed = -1);

		float Compute(float x, float z) override;
	private:
		PerlinNoise perlin;
		Int32 octaves;
		float persistance;
		float lacunarity;
	};

} // namespace Voxel