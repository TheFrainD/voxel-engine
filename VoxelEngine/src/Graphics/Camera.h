#pragma once

// Headers
#include <Config.h>
#include <Core/Log.h>
#include <Core/Game.h>
#include <Core/Event/Event.h>
#include <Core/Event/WindowEvent.h>

#include <glm/glm.hpp>

namespace Voxel
{

	class Camera
	{
	public:
		Camera() : position(glm::vec3(0.0f))
		{
		
		}

		virtual void Setup(const Game* game);

		virtual void SetPosition(const glm::vec3& position)
		{
			this->position = position;
			UpdateView();
		}

		virtual void Translate(const glm::vec3& translation)
		{
			SetPosition(position + translation);
		}

		const glm::vec3& GetPosition() const { return position; }
		const glm::mat4& GetView() const { return view; }
		const glm::mat4& GetProjection() const { return projection; }
	protected:
		virtual void UpdateView() = 0;
		virtual void UpdateProjection(Uint32 windowWidth, Uint32 windowHeight) = 0;

		glm::vec3 position;
		glm::mat4 view;
		glm::mat4 projection;
	};

} // namespace Voxel