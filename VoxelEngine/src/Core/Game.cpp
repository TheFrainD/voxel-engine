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
#include <World/WorldGenerator/ClassicWorldGenerator.h>

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

        const int imgWidth = 128, imgHeight = 128;
        std::vector<unsigned char> continentalnessImageBuffer(imgWidth * imgHeight * 3, 0);
        std::vector<unsigned char> erosionImageBuffer(imgWidth * imgHeight * 3, 0);
        std::vector<unsigned char> peacksImageBuffer(imgWidth * imgHeight * 3, 0);

		auto worldGen = (ClassicWorldGenerator *)World::GetWorldGenerator();
		{
			auto continentalnessNoise = worldGen->GetContinentalnessNoise();
			auto erosionNoise = worldGen->GetErosionNoise();
			auto peaksNoise = worldGen->GetPeaksNoise();

			for (int i = 0; i < imgWidth * imgHeight; ++i) {
				unsigned char cont = continentalnessNoise->Compute(i % imgWidth, i / imgWidth) * 255;
				unsigned char eros = erosionNoise->Compute(i % imgWidth, i / imgWidth) * 255;
				unsigned char peak = peaksNoise->Compute(i % imgWidth, i / imgWidth) * 255;

				continentalnessImageBuffer[i * 3 + 0]        = cont;  // Red channel
				continentalnessImageBuffer[i * 3 + 1]        = cont;  // Green channel
				continentalnessImageBuffer[i * 3 + 2]        = cont;  // Blue channel
				// continentalnessImageBuffer[i * 3 + 3] 		 = 0;

				erosionImageBuffer[i * 3 + 0]        = eros;  // Red channel
				erosionImageBuffer[i * 3 + 1]        = eros;  // Green channel
				erosionImageBuffer[i * 3 + 2]        = eros;  // Blue channel
				// erosionImageBuffer[i * 3 + 3] 		 = 0;

				peacksImageBuffer[i * 3 + 0]        = peak;  // Red channel
				peacksImageBuffer[i * 3 + 1]        = peak;  // Green channel
				peacksImageBuffer[i * 3 + 2]        = peak;  // Blue channel
				// peacksImageBuffer[i * 3 + 3] 		 = 0;
		}
		}

        auto texture1 = Texture::Create(imgWidth, imgHeight);
        texture1->SetData(continentalnessImageBuffer.data(), continentalnessImageBuffer.size(), false);

        auto texture2 = Texture::Create(imgWidth, imgHeight);
        texture2->SetData(erosionImageBuffer.data(), erosionImageBuffer.size(), false);

        auto texture3 = Texture::Create(imgWidth, imgHeight);
        texture3->SetData(peacksImageBuffer.data(), peacksImageBuffer.size(), false);

		float slider1_1 = 0.0f, slider1_2 = 0.0f, slider1_3 = 0.0f;
		float slider2_1 = 0.0f, slider2_2 = 0.0f, slider2_3 = 0.0f;
		float slider3_1 = 0.0f, slider3_2 = 0.0f, slider3_3 = 0.0f;

		int seed = Random::Next();

		auto RegenerateWorld = [&](int seed_) {
			World::Regenerate(seed_);

			auto continentalnessNoise = worldGen->GetContinentalnessNoise();
			auto erosionNoise = worldGen->GetErosionNoise();
			auto peaksNoise = worldGen->GetPeaksNoise();

			for (int i = 0; i < imgWidth * imgHeight; ++i) {
				unsigned char cont = continentalnessNoise->Compute(i % imgWidth, i / imgWidth) * 255;
				unsigned char eros = erosionNoise->Compute(i % imgWidth, i / imgWidth) * 255;
				unsigned char peak = peaksNoise->Compute(i % imgWidth, i / imgWidth) * 255;

				continentalnessImageBuffer[i * 3 + 0]        = cont;  // Red channel
				continentalnessImageBuffer[i * 3 + 1]        = cont;  // Green channel
				continentalnessImageBuffer[i * 3 + 2]        = cont;  // Blue channel
				// continentalnessImageBuffer[i * 3 + 3] 		 = 0;

				erosionImageBuffer[i * 3 + 0]        = eros;  // Red channel
				erosionImageBuffer[i * 3 + 1]        = eros;  // Green channel
				erosionImageBuffer[i * 3 + 2]        = eros;  // Blue channel
				// erosionImageBuffer[i * 3 + 3] 		 = 0;

				peacksImageBuffer[i * 3 + 0]        = peak;  // Red channel
				peacksImageBuffer[i * 3 + 1]        = peak;  // Green channel
				peacksImageBuffer[i * 3 + 2]        = peak;  // Blue channel
				// peacksImageBuffer[i * 3 + 3] 		 = 0;
			}

			texture1->SetData(continentalnessImageBuffer.data(), continentalnessImageBuffer.size(), false);
			texture2->SetData(erosionImageBuffer.data(), erosionImageBuffer.size(), false);
			texture3->SetData(peacksImageBuffer.data(), peacksImageBuffer.size(), false);
		};

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
			ImGui::Begin("Continentalness");
			ImGui::Image((void*)(intptr_t)texture1->GetId(), ImVec2((float)imgWidth, (float)imgHeight));
			ImGui::SliderFloat("Slider 1.1", &slider1_1, 0.0f, 1.0f);
			ImGui::SliderFloat("Slider 1.2", &slider1_2, 0.0f, 1.0f);
			ImGui::SliderFloat("Slider 1.3", &slider1_3, 0.0f, 1.0f);
			if (ImGui::Button("Apply")) {
				RegenerateWorld(seed);
			}
			ImGui::End();

			ImGui::Begin("Erosion");
			ImGui::Image((void*)(intptr_t)texture2->GetId(), ImVec2((float)imgWidth, (float)imgHeight));
			ImGui::SliderFloat("Slider 2.1", &slider2_1, 0.0f, 1.0f);
			ImGui::SliderFloat("Slider 2.2", &slider2_2, 0.0f, 1.0f);
			ImGui::SliderFloat("Slider 2.3", &slider2_3, 0.0f, 1.0f);
			if (ImGui::Button("Apply")) {
				RegenerateWorld(seed);
			}
			ImGui::End();

			ImGui::Begin("Peaks");
			ImGui::Image((void*)(intptr_t)texture3->GetId(), ImVec2((float)imgWidth, (float)imgHeight));
			ImGui::SliderFloat("Slider 3.1", &slider3_1, 0.0f, 1.0f);
			ImGui::SliderFloat("Slider 3.2", &slider3_2, 0.0f, 1.0f);
			ImGui::SliderFloat("Slider 3.3", &slider3_3, 0.0f, 1.0f);
			if (ImGui::Button("Apply")) {
				RegenerateWorld(seed);
			}

			ImGui::Begin("Seed");
			ImGui::InputInt("Enter seed", &seed);
			if (ImGui::Button("Random")) {
				seed = Random::Next();
			}
			ImGui::SameLine();
			if (ImGui::Button("Apply")) {
				RegenerateWorld(seed);
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