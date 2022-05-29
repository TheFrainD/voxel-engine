#pragma once

// Headers
#include <Config.h>

#include <cstdlib>
#include <ctime>

namespace Voxel
{

	class Random
	{
	public:
		static inline void Init()
		{
			srand(time(nullptr));
		}

		static inline Int32 Next()
		{
			return rand();
		}

		static inline Int32 NextRange(Int32 min, Int32 max)
		{
			return ((rand() % ((max)-(min)+1)) + (min));
		}
	};

} // namespace Voxel