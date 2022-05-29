#include <vepch.h>
#include "PerlinNoise.h"

// Headers
#include <Config.h>
#include <World/Noise/Random.h>

#include <FastNoiseLite.h>

namespace Voxel
{
	PerlinNoise::PerlinNoise()
	{
		noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
		noise.SetSeed(Random::Next());
	}

	float PerlinNoise::Compute(float x, float z)
	{
		return (noise.GetNoise(x, z) + 1.0) / 2.0;
	}

} // namespace Voxel