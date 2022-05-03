#pragma once

// Headers
#include <Config.h>
#include <Core/Event/Event.h>

namespace Voxel
{

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(Uint32 width, Uint32 height) :
			width(width), height(height) { }

		Uint32 GetWidth() const { return width; }
		Uint32 GetHeight() const { return height; }

		EVENT_TYPE("WindowResizeEvent");
	private:
		Uint32 width;
		Uint32 height;
	};

	class WindowCloseEvent : public Event
	{
	public:
		EVENT_TYPE("WindowCloseEvent");
	};

} // namespace Voxel