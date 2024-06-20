#pragma once

// Headers
#include <Config.h>
#include <World/WorldGenerator/WorldGenerator.h>

namespace Voxel
{

	class FlatWorldGenerator : public WorldGenerator
	{
	public:
		FlatWorldGenerator(Uint32 size) : WorldGenerator(size)
		{

		}

		void Generate(std::vector<std::shared_ptr<Chunk>>& chunks, int seed = -1) override;
	};

} // namespace Voxel