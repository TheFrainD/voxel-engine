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
#include <World/Noise/Random.h>

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

        const int imgWidth = 256, imgHeight = 256;
        std::vector<unsigned char> imageBuffer1(imgWidth * imgHeight, 0xFF);
        std::vector<unsigned char> imageBuffer2(imgWidth * imgHeight, 0x80);
        std::vector<unsigned char> imageBuffer3(imgWidth * imgHeight, 0x40);

        auto texture1 = Texture::Create(imgWidth, imgHeight);
        texture1->SetData(imageBuffer1.data(), imageBuffer1.size());

        auto texture2 = Texture::Create(imgWidth, imgHeight);
        texture2->SetData(imageBuffer2.data(), imageBuffer2.size());

        auto texture3 = Texture::Create(imgWidth, imgHeight);
        texture3->SetData(imageBuffer3.data(), imageBuffer3.size());

		float slider1_1 = 0.0f, slider1_2 = 0.0f, slider1_3 = 0.0f;
		float slider2_1 = 0.0f, slider2_2 = 0.0f, slider2_3 = 0.0f;
		float slider3_1 = 0.0f, slider3_2 = 0.0f, slider3_3 = 0.0f;

		int seed = Random::Next();

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

			// Start the ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			// Render ImGui content
			ImGui::Begin("Image 1 and Sliders");
			ImGui::Image((void*)(intptr_t)texture1->GetId(), ImVec2((float)imgWidth, (float)imgHeight));
			ImGui::SliderFloat("Slider 1.1", &slider1_1, 0.0f, 1.0f);
			ImGui::SliderFloat("Slider 1.2", &slider1_2, 0.0f, 1.0f);
			ImGui::SliderFloat("Slider 1.3", &slider1_3, 0.0f, 1.0f);
			if (ImGui::Button("Button 1")) {
				World::Regenerate(seed);
			}
			ImGui::End();

			ImGui::Begin("Image 2 and Sliders");
			ImGui::Image((void*)(intptr_t)texture2->GetId(), ImVec2((float)imgWidth, (float)imgHeight));
			ImGui::SliderFloat("Slider 2.1", &slider2_1, 0.0f, 1.0f);
			ImGui::SliderFloat("Slider 2.2", &slider2_2, 0.0f, 1.0f);
			ImGui::SliderFloat("Slider 2.3", &slider2_3, 0.0f, 1.0f);
			if (ImGui::Button("Button 2")) {
				World::Regenerate(seed);
			}
			ImGui::End();

			ImGui::Begin("Image 3 and Sliders");
			ImGui::Image((void*)(intptr_t)texture3->GetId(), ImVec2((float)imgWidth, (float)imgHeight));
			ImGui::SliderFloat("Slider 3.1", &slider3_1, 0.0f, 1.0f);
			ImGui::SliderFloat("Slider 3.2", &slider3_2, 0.0f, 1.0f);
			ImGui::SliderFloat("Slider 3.3", &slider3_3, 0.0f, 1.0f);
			if (ImGui::Button("Button 3")) {
				World::Regenerate(seed);
			}

			ImGui::Begin("Seed");
			ImGui::InputInt("Enter seed", &seed);
			if (ImGui::Button("Random")) {
				seed = Random::Next();
			}

			ImGui::End();

			// Rendering ImGui
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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