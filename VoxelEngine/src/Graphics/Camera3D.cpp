#include <vepch.h>
#include "Camera3D.h"

// Headers
#include <Config.h>
#include <Core/Game.h>
#include <Core/Input.h>
#include <Core/Event/Event.h>
#include <Core/Event/InputEvent.h>
#include <Graphics/Camera.h>

#include <memory>
#include <functional>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define WORLD_UP glm::vec3(0.0f, 1.0f, 0.0f)

namespace Voxel
{

	Camera3D::Camera3D(float fov, float sensitivity) : Camera()
	{
		this->fov = glm::radians(fov);
		this->sensitivity = sensitivity;

		up = glm::vec3(0.0f, 1.0f, 0.0f);
		front = glm::vec3(0.0f, 0.0f, -1.0f);
		right = glm::vec3(1.0f, 0.0f, 0.0f);

		yaw = -math::PI_2;
		pitch = 0.0f;

		EventBus::Subscribe<MouseMovedEvent>(EVENT_CALLBACK(Camera3D::OnMouseMoved, this));
	}

	void Camera3D::UpdateProjection(Uint32 windowWidth, Uint32 windowHeight)
	{
		projection = glm::perspective(fov, (float)windowWidth / (float)windowHeight, 0.1f, 1000.0f);
	}

	void Camera3D::UpdateView()
	{	
		pitch = CLAMP(pitch, -math::PI_2 + 0.01f, math::PI_2 - 0.01f);
		yaw = (yaw < 0 ? math::PI * 2.0f : 0.0f) + fmodf(yaw, math::TAU);

		front = glm::vec3(
			cosf(yaw) * cosf(pitch),
			sinf(pitch),
			sinf(yaw) * cosf(pitch));

		front = glm::normalize(front);
		right = glm::normalize(glm::cross(front, WORLD_UP));
		up = glm::normalize(glm::cross(right, front));

		view = glm::lookAt(position, position + front, up);
	}

	void Camera3D::OnMouseMoved(const Event* event)
	{
		glm::vec2 deltaPosition = EVENT(MouseMovedEvent)->GetPosition() - EVENT(MouseMovedEvent)->GetLastPosition();
		deltaPosition.x = CLAMP(deltaPosition.x, -100.0f, 100.0f);
		deltaPosition.y = CLAMP(deltaPosition.y, -100.0f, 100.0f);
		yaw += deltaPosition.x * sensitivity;
		pitch -= deltaPosition.y * sensitivity;

		UpdateView();
	}

	std::shared_ptr<Camera3D> Camera3D::Create(const Game* game, float fov, float sensitivity)
	{
		auto camera = std::make_shared<Camera3D>(fov, sensitivity);
		camera->Setup(game);
		return camera;
	}

} // namespace Voxel