#include <vepch.h>
#include <Config.h>
#include <Core/Game.h>
#include <Core/Input.h>
#include <Core/Event/Event.h>
#include <Core/Event/InputEvent.h>
#include <Graphics/Shader.h>
#include <Graphics/Texture.h>

#include <glad/glad.h>

GLuint vao;
GLuint vbo;

float vertices[] = {
	-1.0f,-1.0f, 0.0f, 0.0f, 1.0f,
	1.0f,-1.0f, 0.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 0.0f, 0.0f, 0.0f,

	1.0f,-1.0f, 0.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
};

using namespace Voxel;

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
				if (EVENT(KeyPressedEvent)->GetKey() == Key::Space)
				{
					this->Close();
				}
			});

		shader.Load("data/shaders/test.vert", "data/shaders/test.frag");
		texture.Load("data/textures/mario.jpg");

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);

		glClearColor(148.0f / 255, 148.0f / 255, 143.0f / 255, 1.0);
	}

	virtual void Update(float deltaTime) override
	{
		if (Input::IsKeyPressed(Key::Escape))
		{
			this->Close();
		}
	}

	virtual void Render() override
	{
		glClear(GL_COLOR_BUFFER_BIT);

		shader.Use();
		texture.Bind();
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
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