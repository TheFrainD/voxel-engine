#include <vepch.h>
#include "ResourceManager.h"

// Headers
#include <Config.h>
#include <vector>

namespace Voxel::Utils
{

	std::vector<Resource*> ResourceManager::_resources;

	void ResourceManager::Add(Resource* resource)
	{
		_resources.push_back(resource);
	}

	void ResourceManager::Clean()
	{
		for (auto resource : _resources)
		{
			if (resource != nullptr)
			{
				resource->Destroy();
			}
		}
	}

} // namespace Voxel::Utils