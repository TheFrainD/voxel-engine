#include <vepch.h>
#include "OctaveNoise.h"

// Headers
#include <Config.h>
#include <World/Noise/PerlinNoise.h>

namespace Voxel
{

	OctaveNoise::OctaveNoise(Int32 octaves, float persistance, float lacunarity, int seed) :
		perlin(seed), octaves(octaves), persistance(persistance), lacunarity(lacunarity)
	{
		
	}

	float OctaveNoise::Compute(float x, float z)
	{
		float result = 0.0;
		float amplitude = 1.0;
		float amplitudes = 0.0;
		float frequency = 1.0;

		for (Int32 i = 0; i < octaves; i++)
		{
			result += amplitude * perlin.Compute(frequency * x, frequency * z);

			amplitudes += amplitude;
			amplitude *= persistance;
			frequency *= lacunarity;
		}

		result /= amplitudes;

		return result;
	}

} // namespace Voxel