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
#include <World/Chunk/ChunkRenderer.h>
#include <World/Noise/Random.h>
#include <World/WorldGenerator/ClassicWorldGenerator.h>

#include <memory>
#include <functional>

#include <imgui.h>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

using namespace Voxel;

const int imgWidth = 128, imgHeight = 128;
std::vector<unsigned char> continentalnessImageBuffer(imgWidth * imgHeight * 3, 0);
std::vector<unsigned char> erosionImageBuffer(imgWidth * imgHeight * 3, 0);
std::vector<unsigned char> peacksImageBuffer(imgWidth * imgHeight * 3, 0);

ClassicWorldGenerator *worldGen;

int seed = 0;

std::shared_ptr<Texture> texture1;
std::shared_ptr<Texture> texture2;
std::shared_ptr<Texture> texture3;

float slider1_1 = 0.0f, slider1_2 = 0.0f, slider1_3 = 0.0f;
float slider2_1 = 0.0f, slider2_2 = 0.0f, slider2_3 = 0.0f;
float slider3_1 = 0.0f, slider3_2 = 0.0f, slider3_3 = 0.0f;

void RegenerateWorld() {
	World::Regenerate(seed);

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
	VE_LOG_CRITICAL("HERE4");

	texture1->SetData(continentalnessImageBuffer.data(), continentalnessImageBuffer.size(), false);
	texture2->SetData(erosionImageBuffer.data(), erosionImageBuffer.size(), false);
	texture3->SetData(peacksImageBuffer.data(), peacksImageBuffer.size(), false);
}

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

		UI::Add(UIElement::Create(Texture::Create("data/textures/crosshair.png"), { 0.5f, 0.5f }, { 16.0f, 16.0f }));

		Renderer::SetClearColor({ 0.0f, 0.709803f, 0.886274f, 1.0f });

		seed = Random::Next();

		World::Init(this);

		playerController = PlayerController::Create();
		playerController->SetCamera(World::GetCamera());

		EventBus::Subscribe<KeyPressedEvent>([this](const Event* event){
			if (EVENT(KeyPressedEvent)->GetKey() == Key::Escape)
			{
				this->Close();
			}

			if (EVENT(KeyPressedEvent)->GetKey() == Key::F)
			{
				ChunkRenderer::ToggleWireframe();
			}

			if (EVENT(KeyPressedEvent)->GetKey() == Key::V)
			{
				window->SetVSync(!window->GetVSync());
			}

			if (EVENT(KeyPressedEvent)->GetKey() == Key::G)
			{
				Input::SetMouseGrabbed(!Input::GetMouseGrabbed());
				World::GetCamera()->SetActive(!World::GetCamera()->GetActive());
			}
		});

		worldGen = (ClassicWorldGenerator *)World::GetWorldGenerator();
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

				erosionImageBuffer[i * 3 + 0]        = eros;  // Red channel
				erosionImageBuffer[i * 3 + 1]        = eros;  // Green channel
				erosionImageBuffer[i * 3 + 2]        = eros;  // Blue channel

				peacksImageBuffer[i * 3 + 0]        = peak;  // Red channel
				peacksImageBuffer[i * 3 + 1]        = peak;  // Green channel
				peacksImageBuffer[i * 3 + 2]        = peak;  // Blue channel
			}
		}

		texture1 = Texture::Create(imgWidth, imgHeight);
		texture1->SetData(continentalnessImageBuffer.data(), continentalnessImageBuffer.size(), false);
		texture2 = Texture::Create(imgWidth, imgHeight);
		texture2->SetData(erosionImageBuffer.data(), erosionImageBuffer.size(), false);
		texture3 = Texture::Create(imgWidth, imgHeight);
		texture3->SetData(peacksImageBuffer.data(), peacksImageBuffer.size(), false);
	}

	virtual void Update(float deltaTime) override
	{
		World::Update();
		playerController->Update(deltaTime);

		if (Input::IsMouseButtonPressed(MouseButton::ButtonLeft))
		{
			World::SetBlock(70, 10, 10, BlockType::Air);
		}
	}

	virtual void Render() override
	{
		World::Render();
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
			RegenerateWorld();
		}
		ImGui::End();

		ImGui::Begin("Erosion");
		ImGui::Image((void*)(intptr_t)texture2->GetId(), ImVec2((float)imgWidth, (float)imgHeight));
		ImGui::SliderFloat("Slider 2.1", &slider2_1, 0.0f, 1.0f);
		ImGui::SliderFloat("Slider 2.2", &slider2_2, 0.0f, 1.0f);
		ImGui::SliderFloat("Slider 2.3", &slider2_3, 0.0f, 1.0f);
		if (ImGui::Button("Apply")) {
			RegenerateWorld();
		}
		ImGui::End();

		ImGui::Begin("Peaks");
		ImGui::Image((void*)(intptr_t)texture3->GetId(), ImVec2((float)imgWidth, (float)imgHeight));
		ImGui::SliderFloat("Slider 3.1", &slider3_1, 0.0f, 1.0f);
		ImGui::SliderFloat("Slider 3.2", &slider3_2, 0.0f, 1.0f);
		ImGui::SliderFloat("Slider 3.3", &slider3_3, 0.0f, 1.0f);
		if (ImGui::Button("Apply")) {
			RegenerateWorld();
		}

		ImGui::Begin("Seed");
		ImGui::InputInt("Enter seed", &seed);
		if (ImGui::Button("Random")) {
			seed = Random::Next();
		}
		ImGui::SameLine();
		if (ImGui::Button("Apply")) {
			RegenerateWorld();
		}

		ImGui::End();

		// Rendering ImGui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	virtual void Destroy() override
	{
		World::Destroy();
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