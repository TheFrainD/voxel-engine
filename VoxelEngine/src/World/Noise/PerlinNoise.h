#pragma once

// Headers
#include <Config.h>
#include <World/Noise/Noise.h>

#include <FastNoiseLite.h>

namespace Voxel
{

	class PerlinNoise : public Noise
	{
	public:
		PerlinNoise(int seed = -1);

		float Compute(float x, float z) override;
	private:
		FastNoiseLite noise;
	};

} // namespace Voxel