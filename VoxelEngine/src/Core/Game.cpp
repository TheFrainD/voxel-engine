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
#include <Graphics/Texture.h>
#include <UI/UI.h>
#include <World/World.h>

#include <glad/glad.h>

#include <imgui.h>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#include <backends/imgui_impl_glfw.cpp>
#include <backends/imgui_impl_opengl3.cpp>

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

		// Setup ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window->Get(), true);
		ImGui_ImplOpenGL3_Init("#version 410");

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

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		Destroy();

		Utils::ResourceManager::Clean();
	}

	void Game::Close()
	{
		running = false;
	}

} // namespace Voxel