#pragma once

// Headers
#include <Config.h>
#include <World/Noise/Noise.h>
#include <World/Noise/PerlinNoise.h>
#include <World/Noise/OctaveNoise.h>

namespace Voxel
{

	class CombinedNoise : public Noise
	{
	public:
		CombinedNoise(Noise* first, Noise* second);
		~CombinedNoise();

		float Compute(float x, float z) override;
	private:
		Noise* first;
		Noise* second;
	};

} // namespace Voxel