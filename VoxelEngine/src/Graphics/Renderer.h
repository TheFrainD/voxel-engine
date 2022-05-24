#pragma once

// Headers
#include <Config.h>

#include <glm/glm.hpp>

namespace Voxel
{

	class Renderer
	{
	public:
		static void Prepare();
		static void Clear();
		static void SetClearColor(const glm::vec4& color);
		static const glm::vec4& GetClearColor();
	};

} // namespace Voxel