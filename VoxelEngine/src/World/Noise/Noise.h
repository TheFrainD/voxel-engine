#pragma once

// Headers
#include <Config.h>

namespace Voxel
{

	class Noise
	{
	public:
		virtual float Compute(float x, float z) = 0;
	};

} // namespace Voxel