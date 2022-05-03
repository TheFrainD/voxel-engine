#pragma once

// Headers
#include <Config.h>
#include <Core/Event/Event.h>
#include <Core/Input.h>

#include <glm/glm.hpp>

namespace Voxel
{

	class KeyEvent : public Event
	{
	public:
		KeyEvent(KeyCode key) : key(key) { }

		virtual KeyCode GetKey() const { return key; }
	private:
		KeyCode key;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(KeyCode key) : KeyEvent(key) { }

		EVENT_TYPE("KeyPressedEvent";)
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(KeyCode key) : KeyEvent(key) { }

		EVENT_TYPE("KeyReleasedEvent";)
	};

	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(const glm::vec2& position, const glm::vec2& lastPosition) :
			position(position), lastPosition(lastPosition) { }

		virtual glm::vec2 GetPosition() const { return position; }
		virtual glm::vec2 GetLastPosition() const { return lastPosition; }

		EVENT_TYPE("MouseMovedEvent");
	private:
		glm::vec2 position;
		glm::vec2 lastPosition;
	};

	class MouseButtonEvent : public Event
	{
	public:
		MouseButtonEvent(MouseButtonCode button) : button(button) { }

		virtual MouseButtonCode GetButton() const { return button; }
	private:
		MouseButtonCode button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(MouseButtonCode button) : MouseButtonEvent(button) { }

		EVENT_TYPE("MouseButtonPressedEvent");
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(MouseButtonCode button) : MouseButtonEvent(button) { }

		EVENT_TYPE("MouseButtonReleasedEvent");
	};

} // namespace Voxel