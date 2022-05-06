#include <vepch.h>
#include "UI.h"

// Headers
#include <Config.h>
#include <Graphics/Renderer2D.h>
#include <UI/UIElement.h>

#include <memory>
#include <vector>

namespace Voxel
{

	Uint32 UI::_windowWidth;
	Uint32 UI::_windowHeight;
	std::vector<std::shared_ptr<UIElement>> UI::_elements;

	void UI::Render()
	{
		for (auto element : _elements)
		{
			Renderer2D::DrawQuad({ element->GetPosition().x * _windowWidth, element->GetPosition().y * _windowHeight, 0.0f },
				element->GetSize(), element->GetTexture());
		}
	}

	void UI::SetDimensions(Uint32 width, Uint32 height)
	{
		_windowWidth = width;
		_windowHeight = height;
	}

	void UI::Add(const std::shared_ptr<UIElement>& element)
	{
		_elements.push_back(element);
	}


} // namespace Voxel