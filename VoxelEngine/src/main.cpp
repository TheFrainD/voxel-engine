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

#include <glad/glad.h>

float vertices[] = {
	-1.0f,-1.0f, 0.0f, 0.0f, 1.0f,
	1.0f,-1.0f, 0.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 0.0f, 0.0f, 0.0f,

	1.0f,-1.0f, 0.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
};

using namespace Voxel;

VertexArray vertexArray;
VertexBuffer vertexBuffer;

Shader shader;
Texture texture;

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
		EventBus::Subscribe<KeyPressedEvent>([this](const Event* event) {
				if (EVENT(KeyPressedEvent)->GetKey() == Key::Escape)
				{
					this->Close();
				}
			});

		shader.Load("data/shaders/test.vert", "data/shaders/test.frag");
		texture.Load("data/textures/mario.jpg");

		vertexArray.Create();
		vertexBuffer.Create(vertices, sizeof(vertices));
		std::vector<BufferElement> bufferElements{

		};
		vertexBuffer.SetLayout(BufferLayout({
				BufferElement(ShaderDataType::Float3, "position"),
				BufferElement(ShaderDataType::Float2, "uv")
			}));
		vertexArray.AddVertexBuffer(vertexBuffer);

		glClearColor(148.0f / 255, 148.0f / 255, 143.0f / 255, 1.0);
	}

	virtual void Update(float deltaTime) override
	{

	}

	virtual void Render() override
	{
		glClear(GL_COLOR_BUFFER_BIT);

		shader.Use();
		texture.Bind();
		vertexArray.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
		vertexArray.Unbind();
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