#include <vepch.h>
#include "ChunkRenderer.h"

// Headers
#include <Config.h>
#include <Graphics/Texture.h>
#include <Graphics/Shader.h>
#include <World/Chunk/ChunkMesh.h>

#include <memory>

#include <glm/glm.hpp>
#include <glad/glad.h>

namespace Voxel
{

	struct ChunkRenderer_Data
	{
		std::shared_ptr<Shader> shader;
		std::shared_ptr<Texture> texture;
		std::shared_ptr<Camera3D> camera;
	};

	static ChunkRenderer_Data _data;

	void ChunkRenderer::Init()
	{
		_data.shader = Shader::Create("data/shaders/Chunk.vert", "data/shaders/Chunk.frag");
		_data.texture = Texture::Create("data/textures/terrain.png");

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void ChunkRenderer::Render(const std::shared_ptr<ChunkMesh>& mesh)
	{	
		_data.shader->Use();
		_data.texture->Bind();

		_data.shader->UniformMat4("u_projection", _data.camera->GetProjection());
		_data.shader->UniformMat4("u_view", _data.camera->GetView());
		_data.shader->UniformMat4("u_model", mesh->GetModel());

		mesh->Bind();

		glDrawElements(GL_TRIANGLES, mesh->elements.size(), GL_UNSIGNED_INT, 0);
	}

	void ChunkRenderer::SetCurrentCamera(const std::shared_ptr<Camera3D>& camera)
	{
		if (_data.camera)
		{
			_data.camera->SetCurrent(false);
		}
		_data.camera = camera;
		camera->SetCurrent(true);
	}

	const std::shared_ptr<Camera3D>& ChunkRenderer::GetCurrentCamera()
	{
		return _data.camera;
	}

} // namespace Voxel