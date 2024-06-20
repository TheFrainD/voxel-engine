#include <vepch.h>
#include "Renderer2D.h"

// Headers
#include <Config.h>
#include <Graphics/Texture.h>
#include <Graphics/Shader.h>
#include <Graphics/VertexArray.h>
#include <Graphics/Buffer.h>
#include <Graphics/Camera2D.h>

#include <array>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

#define MAX_QUAD_COUNT		1000
#define MAX_VERTEX_COUNT	MAX_QUAD_COUNT * 4
#define MAX_INDEX_COUNT		MAX_QUAD_COUNT * 6
#define MAX_TEXTURES		2

namespace Voxel
{

	struct QuadVertex
	{
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 uv;
		float texID;
	};

	struct Renderer2D_Data
	{
		std::shared_ptr<VertexArray> vertexArray;
		std::shared_ptr<VertexBuffer> vertexBuffer;
		std::shared_ptr<ElementBuffer> elementBuffer;

		std::shared_ptr<Shader> shader;

		std::shared_ptr<Texture> whiteTexture;
		Uint32 whiteTextureSlot = 0;

		Uint32 indexCount = 0;

		QuadVertex* quadBuffer = nullptr;
		QuadVertex* quadBuffer_ptr = nullptr;

		std::array<std::shared_ptr<Texture>, MAX_TEXTURES> textureSlots;
		Uint32 textureSlotIndex = 1;

		glm::vec4 quadVertexPositions[4];

		std::shared_ptr<Camera2D> camera;

		Renderer2D::Statistics statistics;
	};

	static Renderer2D_Data _data;

	void Renderer2D::Init()
	{
		_data.vertexArray = VertexArray::Create();

		_data.vertexBuffer = VertexBuffer::Create(MAX_VERTEX_COUNT * sizeof(QuadVertex));
		_data.vertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_position"	},
			{ ShaderDataType::Float4, "a_color"		},
			{ ShaderDataType::Float2, "a_uv"		},
			{ ShaderDataType::Float,  "a_texID"		}
		});
		_data.vertexArray->AddVertexBuffer(_data.vertexBuffer);

		_data.quadBuffer = new QuadVertex[MAX_VERTEX_COUNT];

		Uint32* indices = new Uint32[MAX_INDEX_COUNT];

		Uint32 offset = 0;
		for (Uint32 i = 0; i < MAX_INDEX_COUNT; i += 6)
		{
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;
			
			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}

		_data.elementBuffer = ElementBuffer::Create(indices, MAX_INDEX_COUNT);
		_data.vertexArray->SetElementBuffer(_data.elementBuffer);
		delete[] indices;

		_data.whiteTexture = Texture::Create(1, 1);
		Uint32 whiteTextureData = 0xffffffff;
		_data.whiteTexture->SetData(&whiteTextureData, sizeof(Uint32));
		_data.textureSlots[0] = _data.whiteTexture;

		_data.shader = Shader::Create("data/shaders/Batch2D.vert", "data/shaders/Batch2D.frag");
		_data.shader->UniformMat4("u_projection", _data.camera->GetProjection());
		_data.shader->UniformMat4("u_view", _data.camera->GetView());

		int* sampler = new int[MAX_TEXTURES];
		for (Uint32 i = 0; i < MAX_TEXTURES; i++) {
			sampler[i] = i;
		}
		_data.shader->UniformIntArray("u_image", MAX_TEXTURES, sampler);
		delete[] sampler;

		_data.quadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		_data.quadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		_data.quadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		_data.quadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown()
	{
		delete[] _data.quadBuffer;
	}

	void Renderer2D::BeginBatch()
	{
		_data.quadBuffer_ptr = _data.quadBuffer;
		
		_data.indexCount = 0;
		_data.textureSlotIndex = 1;
	}

	void Renderer2D::NextBatch()
	{
		Flush();
		BeginBatch();
	}

	void Renderer2D::Flush()
	{
		if (_data.indexCount == 0)
		{
			return;
		}

		GLsizeiptr size = (Uint8*)_data.quadBuffer_ptr - (Uint8*)_data.quadBuffer;
		_data.vertexBuffer->SetData(_data.quadBuffer, size);
		
		for (Uint32 i = 0; i < _data.textureSlotIndex; i++)
		{
			_data.textureSlots[i]->Bind(i);
		}

		_data.shader->Use();

		_data.vertexArray->Bind();
		_data.elementBuffer->Bind();

		glDepthFunc(GL_ALWAYS);

		glDrawElements(GL_TRIANGLES, _data.indexCount, GL_UNSIGNED_INT, nullptr);

		_data.statistics.drawCalls++;
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		if (_data.indexCount >= MAX_INDEX_COUNT)
		{
			NextBatch();
		}

		const float texID = 0.0f;
		const glm::vec2 uvs[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (Uint32 i = 0; i < 4; i++)
		{
			_data.quadBuffer_ptr->position = transform * _data.quadVertexPositions[i];
			_data.quadBuffer_ptr->color = color;
			_data.quadBuffer_ptr->uv = uvs[i];
			_data.quadBuffer_ptr->texID = texID;
			_data.quadBuffer_ptr++;
		}

		_data.indexCount += 6;
		_data.statistics.quadCount++;
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture>& texture)
	{
		if (_data.indexCount >= MAX_INDEX_COUNT)
		{
			NextBatch();
		}

		const glm::vec2 uvs[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		float texID = 0.0f;
		for (Uint32 i = 1; i < _data.textureSlotIndex; i++)
		{
			if (*_data.textureSlots[i] == *texture)
			{
				texID = (float)i;
				break;
			}
		}

		if (texID == 0.0f)
		{
			if (_data.textureSlotIndex >= MAX_TEXTURES)
			{
				NextBatch();
			}

			texID = (float)_data.textureSlotIndex;
			_data.textureSlots[_data.textureSlotIndex] = texture;
			_data.textureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (Uint32 i = 0; i < 4; i++)
		{
			_data.quadBuffer_ptr->position = transform * _data.quadVertexPositions[i];
			_data.quadBuffer_ptr->color = glm::vec4(1.0f);
			_data.quadBuffer_ptr->uv = uvs[i];
			_data.quadBuffer_ptr->texID = texID;
			_data.quadBuffer_ptr++;
		}

		_data.indexCount += 6;
		_data.statistics.quadCount++;
	}

	void Renderer2D::SetCurrentCamera(const std::shared_ptr<Camera2D>& camera)
	{
		
		if (_data.camera)
		{
			_data.camera->SetCurrent(false);
		}
		_data.camera = camera;
		camera->SetCurrent(true);
	}

	const std::shared_ptr<Camera2D>& Renderer2D::GetCurrentCamera()
	{
		return _data.camera;
	}

	void Renderer2D::SendView(const glm::mat4& view)
	{
		_data.shader->UniformMat4("u_view", _data.camera->GetView());
	}

	void Renderer2D::SendProjection(const glm::mat4& projection)
	{
		_data.shader->UniformMat4("u_projection", _data.camera->GetProjection());
	}

	void Renderer2D::ResetStats()
	{
		memset(&_data.statistics, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return _data.statistics;
	}

} // namespace Voxel