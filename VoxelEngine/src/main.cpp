#include <vepch.h>
#include <Config.h>
#include <Core/Game.h>
#include <Core/Input.h>
#include <Core/Event/Event.h>
#include <Core/Event/InputEvent.h>
#include <Graphics/Shader.h>
#include <Graphics/Texture.h>
#include <Graphics/Buffer.h>
#include <Graphics/VertexArray.h>
#include <Graphics/Camera2D.h>
#include <Graphics/Renderer2D.h>

#include <memory>
#include <cmath>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace Voxel;

std::shared_ptr<Texture> texture;

std::shared_ptr<Camera2D> camera;

class VoxelEngine : public Game
{
public:
	VoxelEngine()
	{

	}

	~VoxelEngine()
	{

	}

	virtual void Start() override
	{
		EventBus::Subscribe<KeyPressedEvent>([this](const Event* event){
			if (EVENT(KeyPressedEvent)->GetKey() == Key::Escape)
			{
				this->Close();
			}
		});

		texture = Texture::Create("data/textures/mario.jpg");

		camera = Camera2D::Create(this);
		Renderer2D::SetCurrentCamera(camera);
	}

	virtual void Update(float deltaTime) override
	{

	}

	virtual void Render() override
	{
		for (int j = 0; j < 28; j++)
		{
			for (int i = 0; i < 49; i++)
			{
				Renderer2D::DrawQuad({ 13.0f + (26.0f * i), 13.0f + (26.0f * j), 0.0f }, { 25.0f, 25.0f }, texture);
			}
		}
	}

	virtual void Destroy() override
	{
		
	}
};

MAIN
{
	VoxelEngine* engine = new VoxelEngine();
	engine->Run(1280, 720, "VoxelEngine");
	delete engine;
	return 0;
}