#pragma once

// Headers
#include <Config.h>
#include <Core/Window.h>

#include <memory>

namespace Voxel 
{

	class Game
	{
	public:
		Game();
		virtual ~Game();

		virtual void Start() = 0;
		virtual void Update(float deltaTime) = 0;
		virtual void Render() = 0;
		virtual void Destroy() = 0;

		void Run(Uint32 width, Uint32 height, const char* title);
		void Close();
	private:
		std::unique_ptr<Window> window;
		bool running;
	};

} // namespace Voxel