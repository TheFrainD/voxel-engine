#include <vepch.h>
#include <Config.h>
#include <Core/Game.h>
#include <Core/Input.h>
#include <Core/Log.h>
#include <Core/Event/Event.h>
#include <Core/Event/InputEvent.h>
#include <Graphics/Texture.h>
#include <Graphics/Renderer2D.h>
#include <Graphics/Renderer.h>
#include <Graphics/Camera2D.h>
#include <UI/UI.h>
#include <World/World.h>
#include <Player/PlayerController.h>

#include <memory>

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
		EventBus::Subscribe<KeyPressedEvent>([this](const Event* event){
			if (EVENT(KeyPressedEvent)->GetKey() == Key::Escape)
			{
				this->Close();
			}
		});

		UI::Add(UIElement::Create(Texture::Create("data/textures/crosshair.png"), { 0.5f, 0.5f }, { 16.0f, 16.0f }));

		Renderer::SetClearColor({ 0.0f, 0.709803f, 0.886274f, 1.0f });

		World::Init(this);

		playerController = PlayerController::Create();
		playerController->SetCamera(World::GetCamera());
	}

	virtual void Update(float deltaTime) override
	{
		World::Update();
		playerController->Update(deltaTime);
	}

	virtual void Render() override
	{
		World::Render();
	}

	virtual void Destroy() override
	{
		
	}

private:
	std::shared_ptr<PlayerController> playerController;
};

MAIN
{
	VoxelEngine* engine = new VoxelEngine();
	engine->Run(1280, 720, "VoxelEngine");
	delete engine;
	return 0;
}