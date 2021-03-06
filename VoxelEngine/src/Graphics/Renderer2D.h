#pragma once

// Headers
#include <Config.h>
#include <Graphics/Texture.h>
#include <Graphics/Camera2D.h>

#include <memory>

#include <glm/glm.hpp>

namespace Voxel
{

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginBatch();
		static void NextBatch();
		static void Flush();float texID = 0.0f;

		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture>& texture);

		static void SetCurrentCamera(const std::shared_ptr<Camera2D>& camera);
		static const std::shared_ptr<Camera2D>& GetCurrentCamera();

		static void SendView(const glm::mat4& view);
		static void SendProjection(const glm::mat4& projection);

		struct Statistics
		{
			Uint32 drawCalls = 0;
			Uint32 quadCount = 0;
		};

		static void ResetStats();
		static Statistics GetStats();
	};

} // namespace Voxel