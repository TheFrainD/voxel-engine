#pragma once

// Headers
#include <Config.h>
#include <Core/Game.h>
#include <Core/Event/Event.h>
#include <Graphics/Camera.h>

#include <memory>

#include <glm/glm.hpp>

namespace Voxel
{

	class Camera3D : public Camera
	{
	public:
		Camera3D(float fov, float sensitivity);

		const glm::vec3& GetFront() const { return front; }
		const glm::vec3& GetRight() const { return right; }
		const glm::vec3& GetUp() const { return up; }

		void Update();

		void OnMouseMoved(const Event* event);

		static std::shared_ptr<Camera3D> Create(const Game* game, float fov, float sensitivity);
	protected:
		float fov;
		float sensitivity;

		float yaw;
		float pitch;

		glm::vec3 front;
		glm::vec3 right;
		glm::vec3 up;

		virtual void UpdateView() override;
		virtual void UpdateProjection(Uint32 windowWidth, Uint32 windowHeight) override;
	};

} // namespace Voxel