#include <vepch.h>
#include <Config.h>
#include <Core/Game.h>
#include <Core/Input.h>
#include <Core/Event/Event.h>
#include <Core/Event/InputEvent.h>
#include <Graphics/Texture.h>
#include <Graphics/Renderer2D.h>
#include <Graphics/Camera2D.h>
#include <UI/UI.h>

#include <memory>

using namespace Voxel;

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

		UI::Add(UIElement::Create(Texture::Create("data/textures/crosshair.png"), { 0.5f, 0.5f }, { 16.0f, 16.0f }));

		camera = Camera2D::Create(this);
		Renderer2D::SetCurrentCamera(camera);
	}

	virtual void Update(float deltaTime) override
	{

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