#include <vepch.h>
#include "Game.h"

// Headers
#include <Config.h>
#include <Core/Log.h>
#include <Core/Window.h>

#include <memory>
#include <functional>

namespace Voxel
{

	Game::Game()
	{
		running = true;
	}

	Game::~Game()
	{

	}

	void Game::Run(Uint32 width, Uint32 height, const char* title)
	{
		Start();

		Log::Init("log");
		window = std::unique_ptr<Window>(new Window(width, height, title));

		while (running)
		{
			window->Update();

			Update(0.0f);
			Render();
		}

		Destroy();
	}

	void Game::Close()
	{
		running = false;
	}

} // namespace Voxel