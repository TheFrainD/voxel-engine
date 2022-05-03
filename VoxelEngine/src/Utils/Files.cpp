#include <vepch.h>
#include "Files.h"

// Headers
#include <Config.h>
#include <Core/Log.h>

#include <fstream>
#include <sstream>

namespace Voxel::Utils
{

	std::string ReadFile(const std::string& filepath)
	{
		std::ifstream file(filepath);
		if (!file.is_open())
		{
			VE_LOG_ERROR("Failed to read file at \"{}\"", filepath);
			return "";
		}
		
		std::stringstream buffer;
		buffer << file.rdbuf();

		return buffer.str();
	}

} // namespace Voxel::Utils