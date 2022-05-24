#include <vepch.h>
#include "PlayerController.h"

// Headers
#include <Config.h>
#include <Graphics/Camera3D.h>
#include <Core/Input.h>

#include <memory>

#include <glm/glm.hpp>

namespace Voxel
{
	PlayerController::PlayerController()
	{
		Input::SetMouseGrabbed(true);
	}


	void PlayerController::Update(float deltaTime)
	{
		if (Input::IsKeyPressed(Key::W))
		{
			camera->Translate(camera->GetFront() * (float)deltaTime * speed);
		}
		else if (Input::IsKeyPressed(Key::S))
		{
			camera->Translate(camera->GetFront() * (float)deltaTime * -speed);
		}

		if (Input::IsKeyPressed(Key::D))
		{
			camera->Translate(camera->GetRight() * (float)deltaTime * speed);
		}
		else if (Input::IsKeyPressed(Key::A)) {
			camera->Translate(camera->GetRight() * (float)deltaTime * -speed);
		}
	}

	void PlayerController::SetCamera(const std::shared_ptr<Camera3D>& camera)
	{
		this->camera = camera;
	}

	std::shared_ptr<PlayerController> PlayerController::Create()
	{
		return std::make_shared<PlayerController>();
	}

} // namespace Voxel