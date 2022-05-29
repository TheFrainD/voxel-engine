#pragma once

// Headers
#include <Config.h>
#include <World/WorldGenerator/WorldGenerator.h>

namespace Voxel
{

	class ClassicWorldGenerator : public WorldGenerator
	{
	public:
		ClassicWorldGenerator(Uint32 size) : WorldGenerator(size)
		{

		}

		void Generate(std::vector<std::shared_ptr<Chunk>>& chunks) override;
	private:
		void GenerateHeightmap(Uint32* heightMap, Uint32 width, Uint32 depth);

		Uint32 waterLevel = 32;
	};

} // namespace Voxel