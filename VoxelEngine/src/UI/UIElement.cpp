#include <vepch.h>
#include "UIElement.h"

// Headers
#include <Config.h>
#include <Graphics/Texture.h>

#include <memory>

#include <glm/glm.hpp>

namespace Voxel
{

	std::shared_ptr<UIElement> UIElement::Create(const std::shared_ptr<Texture>& texture,
		const glm::vec2 position, const glm::vec2 size)
	{
		return std::make_shared<UIElement>(texture, position, size);
	}

} // namespace Voxel