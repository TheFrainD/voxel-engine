#include <vepch.h>
#include "Game.h"

// Headers
#include <Config.h>
#include <Core/Log.h>
#include <Core/Window.h>
#include <Core/Input.h>
#include <Core/Event/Event.h>
#include <Core/Event/WindowEvent.h>
#include <Utils/ResourceManager.h>
#include <Graphics/Renderer2D.h>
#include <Graphics/Renderer.h>
#include <UI/UI.h>

#include <memory>
#include <functional>

namespace Voxel
{

	Game::Game()
	{
		Log::Init("log");
		running = true;

		EventBus::Subscribe<WindowCloseEvent>([this](const Event* event) {
			this->Close();
		});

		EventBus::Subscribe<WindowResizeEvent>([this](const Event* event) {
			UI::SetDimensions(EVENT(WindowResizeEvent)->GetWidth(), EVENT(WindowResizeEvent)->GetHeight());
		});
	}

	Game::~Game()
	{

	}

	void Game::Run(Uint32 width, Uint32 height, const char* title)
	{
		window = std::unique_ptr<Window>(new Window(width, height, title));

		Input::Init(window->Get());
		Renderer::Prepare();

		Start();

		UI::Init(this);
		UI::SetDimensions(width, height);

		Renderer2D::Init();

		float deltaTime = 0;
		float currentTime = 0;
		float lastTime = 0;
		while (running)
		{
			currentTime = window->GetTime();
			deltaTime = currentTime - lastTime;
			lastTime = currentTime;

			window->Update();

			Update(deltaTime);
			
			Renderer::Clear();
			Renderer2D::ResetStats();
			Renderer2D::BeginBatch();
			UI::Render();

			Render();

			Renderer2D::Flush();
		}

		Destroy();

		Utils::ResourceManager::Clean();
	}

	void Game::Close()
	{
		running = false;
	}

} // namespace Voxel