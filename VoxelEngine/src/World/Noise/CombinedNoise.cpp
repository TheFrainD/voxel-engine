#include <vepch.h>
#include "OctaveNoise.h"

// Headers
#include <Config.h>
#include <World/Noise/PerlinNoise.h>
#include <World/Noise/OctaveNoise.h>
#include "CombinedNoise.h"

namespace Voxel
{

	CombinedNoise::CombinedNoise(Noise* first, Noise* second)
		: first(first), second(second)
	{

	}

	CombinedNoise::~CombinedNoise()
	{
		delete first;
		delete second;
	}

	float CombinedNoise::Compute(float x, float z)
	{
		return first->Compute(x + second->Compute(x, z), z);
	}

} // namespace Voxel
