#pragma once

// Headers
#include <Config.h>
#include <Core/Game.h>
#include <Graphics/Camera.h>

#include <memory>

#include <glm/glm.hpp>

namespace Voxel
{

	class Camera2D : public Camera
	{
	public:
		static std::shared_ptr<Camera2D> Create(const Game* game);

		Camera2D() : Camera()
		{

		}

	protected:
		virtual void UpdateView() override;
		virtual void UpdateProjection(Uint32 windowWidth, Uint32 windowHeight) override;
	};

} // namespace Voxel