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

#include <memory>
#include <functional>

namespace Voxel
{

	Game::Game()
	{
		Log::Init("log");
		running = true;

		EventBus::Subscribe<WindowCloseEvent>([this](const Event* event)
			{
				VE_LOG_INFO("Window closed!");
				this->Close();
			});
	}

	Game::~Game()
	{

	}

	void Game::Run(Uint32 width, Uint32 height, const char* title)
	{
		window = std::unique_ptr<Window>(new Window(width, height, title));
		Input::Init(window->Get());

		Start();

		while (running)
		{
			window->Update();

			Update(0.0f);
			Render();
		}

		Destroy();

		Utils::ResourceManager::Clean();
	}

	void Game::Close()
	{
		running = false;
	}

} // namespace Voxel