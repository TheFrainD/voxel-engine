#pragma once

// Headers
#include <Config.h>
#include <Graphics/Camera3D.h>

#include <memory>

#include <glm/glm.hpp>

namespace Voxel
{

	class PlayerController
	{
	public:
		PlayerController();

		void Update(float deltaTime);

		void SetCamera(const std::shared_ptr<Camera3D>& camera);

		static std::shared_ptr<PlayerController> Create();
	private:
		std::shared_ptr<Camera3D> camera;

		glm::vec3 position = glm::vec3(0.0f);
		float speed = 15.0f;
	};

} // namespace Voxel