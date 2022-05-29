#pragma once

// Headers
#include <Config.h>
#include <Graphics/Camera3D.h>
#include <World/Chunk/ChunkMesh.h>

#include <memory>

namespace Voxel
{

	class ChunkRenderer
	{
	public:
		static void Init();

		static void Prepare();
		static void Render(const std::shared_ptr<ChunkMesh>& mesh);
		static void End();

		static void SetCurrentCamera(const std::shared_ptr<Camera3D>& camera);
		static const std::shared_ptr<Camera3D>& GetCurrentCamera();

		static void ToggleWireframe();
	};

} // namespace Voxel