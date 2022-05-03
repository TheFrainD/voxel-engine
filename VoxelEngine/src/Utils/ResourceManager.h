#pragma once

// Headers
#include <Config.h>

#include <vector>

namespace Voxel::Utils
{

	class Resource
	{
	public:
		virtual void Destroy() = 0;
	};

	class ResourceManager
	{
	public:
		static void Add(Resource* resource);
		static void Clean();
	private:
		static std::vector<Resource*> _resources;
	};

} // namespace Voxel::Utils