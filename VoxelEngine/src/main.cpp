#include <vepch.h>
#include <Config.h>
#include <Core/Game.h>
#include <Core/Input.h>
#include <Core/Event/Event.h>
#include <Core/Event/InputEvent.h>

using namespace Voxel;

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