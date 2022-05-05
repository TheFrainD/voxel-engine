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

#include <memory>
#include <functional>

#include <glad/glad.h>

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

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		// enable depth test
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_ALWAYS);

		// enable blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		Start();

		Renderer2D::Init();

		while (running)
		{
			window->Update();

			Update(0.0f);
			
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			Renderer2D::ResetStats();
			Renderer2D::BeginBatch();
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