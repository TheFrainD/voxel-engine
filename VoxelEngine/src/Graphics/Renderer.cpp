#include <vepch.h>
#include "Renderer.h"

// Headers
#include <Config.h>

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Voxel
{

	struct RendererData
	{
		glm::vec4 clearColor;
	};

	static RendererData _data;

	void Renderer::Prepare()
	{
		SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
		
		// enable depth test
		glEnable(GL_DEPTH_TEST);

		// enable blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void Renderer::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.x, color.y, color.z, color.w);
		_data.clearColor = color;
	}

	const glm::vec4& Renderer::GetClearColor()
	{
		return _data.clearColor;
	}

} // namespace Voxel