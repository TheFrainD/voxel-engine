#include <vepch.h>
#include "PerlinNoise.h"

// Headers
#include <Config.h>
#include <World/Noise/Random.h>

#include <FastNoiseLite.h>

namespace Voxel
{
	PerlinNoise::PerlinNoise(int seed)
	{
		noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
		if (seed == -1) {
			seed = Random::Next();
		}
		noise.SetSeed(seed);
	}

	float PerlinNoise::Compute(float x, float z)
	{
		return (noise.GetNoise(x, z) + 1.0) / 2.0;
	}

} // namespace Voxel