#pragma once

// Headers
#include <Config.h>
#include <World/Chunk/Chunk.h>

#include <vector>
#include <memory>

namespace Voxel
{

	class WorldGenerator
	{
	public:
		WorldGenerator(Uint32 size)
			: size(size) { }
		
		virtual ~WorldGenerator() = default;

		virtual void Generate(std::vector<std::shared_ptr<Chunk>>& chunks, int seed = -1) = 0;
	private:
		Uint32 size;
	};

} // namespace Voxel