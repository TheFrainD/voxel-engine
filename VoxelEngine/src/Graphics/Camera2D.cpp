#include <vepch.h>
#include "Camera2D.h"

// Headers
#include <Config.h>
#include <Core/Game.h>
#include <Graphics/Camera.h>
#include <Graphics/Renderer2D.h>

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Voxel
{

	std::shared_ptr<Camera2D> Camera2D::Create(const Game* game)
	{
		std::shared_ptr<Camera2D> camera = std::make_shared<Camera2D>();
		camera->Setup(game);
		return camera;
	}

	void Camera2D::UpdateView()
	{
		view = glm::translate(glm::mat4(1.0f), position);

		if (current)
		{
			Renderer2D::SendView(view);
		}
	}

	void Camera2D::UpdateProjection(Uint32 windowWidth, Uint32 windowHeight)
	{
		projection = glm::ortho(0.0f, (float)windowWidth, (float)windowHeight, 0.0f, 0.0f, 100.0f);

		if (current)
		{
			Renderer2D::SendProjection(projection);
		}
	}

} // namespace Voxel