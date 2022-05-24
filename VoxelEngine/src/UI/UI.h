#pragma once

// Headers
#include <Config.h>
#include <Core/Game.h>
#include <UI/UIElement.h>
#include <Graphics/Camera2D.h>

#include <memory>
#include <vector>


namespace Voxel
{

	class UI
	{
	public:
		static void Init(const Game* game);
		static void SetDimensions(Uint32 width, Uint32 height);
		static void Render();
		static void Add(const std::shared_ptr<UIElement>& element);
	private:
		static Uint32 _windowWidth;
		static Uint32 _windowHeight;

		static std::vector<std::shared_ptr<UIElement>> _elements;

		static std::shared_ptr<Camera2D> _camera;
	};

} // namespace Voxel