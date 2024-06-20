#include <vepch.h>
#include "PlayerController.h"

// Headers
#include <Config.h>
#include <Graphics/Camera3D.h>
#include <Core/Input.h>
#include <World/World.h>

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
		float result_speed = speed;

		if (Input::IsKeyPressed(Key::LeftControl))
		{
			result_speed *= 1.5f;
		}
		
		if (Input::IsKeyPressed(Key::W))
		{
			camera->Translate(camera->GetFront() * (float)deltaTime * result_speed);
		}
		else if (Input::IsKeyPressed(Key::S))
		{
			camera->Translate(camera->GetFront() * (float)deltaTime * -result_speed);
		}

		if (Input::IsKeyPressed(Key::D))
		{
			camera->Translate(camera->GetRight() * (float)deltaTime * result_speed);
		}
		else if (Input::IsKeyPressed(Key::A)) {
			camera->Translate(camera->GetRight() * (float)deltaTime * -result_speed);
		}

		{
			glm::vec3 end;
			glm::vec3 norm;
			glm::vec3 iend;
			Block* block = World::RayCast(position, camera->GetFront(), 10.0f, end, norm, iend);
			if (block != nullptr)
			{
				if (Input::IsMouseButtonPressed(MouseButton::ButtonLeft))
				{
					World::SetBlock((int)iend.x, (int)iend.y, (int)iend.z, BlockType::Air);
				}
				else if (Input::IsMouseButtonPressed(MouseButton::ButtonRight))
				{
					World::SetBlock((int)(iend.x) + (int)(norm.x), (int)(iend.y) + (int)(norm.y), (int)(iend.z) + (int)(norm.z), 2);
				}
			}

			// if (Input::IsMouseButtonPressed(MouseButton::ButtonLeft))
			// {
			// 	VE_LOG_INFO("Ray cast end: {} {} {}", iend.x, iend.y, iend.z);
			// }
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