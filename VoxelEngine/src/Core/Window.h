#pragma once

// Headers
#include <Config.h>
#include <string>

// Forward declaration
struct GLFWwindow;

namespace Voxel
{

	class Window
	{
	public:
		Window(Uint32 width, Uint32 height, const std::string& title);
		virtual ~Window();

		void Update();

		Uint32 GetWidth() const { return data.width; }
		Uint32 GetHeight() const { return data.height; }

		void SetVSync(bool vsync);
		bool GetVSync() const { return data.vsync; }
	private:
		GLFWwindow* window;

		struct WindowData
		{
			Uint32 width, height;
			std::string title;
			bool vsync;
		};

		WindowData data;

		void Init();
		void Shutdown();
	};

} // namespace Voxel