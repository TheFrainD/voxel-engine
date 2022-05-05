#include <vepch.h>
#include "Camera.h"

// Headers
#include <Config.h>
#include <Core/Log.h>
#include <Core/Game.h>
#include <Core/Event/Event.h>
#include <Core/Event/WindowEvent.h>

#include <glm/glm.hpp>

namespace Voxel
{

	void Camera::Setup(const Game* game)
	{
		UpdateProjection(game->GetWindowWidth(), game->GetWindowHeight());
		UpdateView();

		EventBus::Subscribe<WindowResizeEvent>([this](const Event* event)
			{
				UpdateProjection(EVENT(WindowResizeEvent)->GetWidth(), EVENT(WindowResizeEvent)->GetHeight());
			}
		);
	}

} // namespace Voxel