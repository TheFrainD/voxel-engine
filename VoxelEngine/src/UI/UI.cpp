#include <vepch.h>
#include "UI.h"

// Headers
#include <Config.h>
#include <Core/Game.h>
#include <Graphics/Renderer2D.h>
#include <Graphics/Camera2D.h>
#include <UI/UIElement.h>

#include <memory>
#include <vector>

namespace Voxel
{

	Uint32 UI::_windowWidth;
	Uint32 UI::_windowHeight;
	std::vector<std::shared_ptr<UIElement>> UI::_elements;
	std::shared_ptr<Camera2D> UI::_camera;

	void UI::Init(const Game* game)
	{
		_camera = Camera2D::Create(game);
		Renderer2D::SetCurrentCamera(_camera);
	}

	void UI::Render()
	{
		if (!_camera->IsCurrent())
		{
			Renderer2D::SetCurrentCamera(_camera);
		}
		
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