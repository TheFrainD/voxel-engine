#pragma once

// Headers
#include <Config.h>
#include <Graphics/Texture.h>

#include <memory>

#include <glm/glm.hpp>

namespace Voxel
{

	class UIElement
	{
	public:
		UIElement(const std::shared_ptr<Texture>& texture, const glm::vec2 position,
			const glm::vec2 size) : texture(texture), position(position), size(size)
		{

		}

		void SetPosition(const glm::vec2& position) { this->position = position; }
		const glm::vec2& GetPosition() const { return position; }
		 
		void SetSize(const glm::vec2& size) { this->size = size; }
		const glm::vec2& GetSize() const { return size; }

		void SetTexture(const std::shared_ptr<Texture>& texture) { this->texture = texture; }
		const std::shared_ptr<Texture>& GetTexture() const { return texture; }

		static std::shared_ptr<UIElement> Create(const std::shared_ptr<Texture>& texture,
			const glm::vec2 position, const glm::vec2 size);
	private:
		std::shared_ptr<Texture> texture;
		glm::vec2 position;
		glm::vec2 size;
	};

} // namespace Voxel