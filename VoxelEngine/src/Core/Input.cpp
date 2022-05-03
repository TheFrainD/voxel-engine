#include <vepch.h>
#include "Input.h"

// Headers
#include <Config.h>
#include <Core/Window.h>
#include <Core/Log.h>
#include <Core/Event/Event.h>
#include <Core/Event/InputEvent.h>

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (Voxel::Input::_keyboard.keys[key] != action)
	{
		Voxel::Input::_keyboard.keys[key] = action;
		Voxel::Input::_keyboard.lastKey = key;

		if (action)
		{
			Voxel::EventBus::Post(new Voxel::KeyPressedEvent(key));
		}
		else
		{
			Voxel::EventBus::Post(new Voxel::KeyReleasedEvent(key));
		}
	}
}

void mouse_callback(GLFWwindow* window, int button, int action, int mode)
{
	if (Voxel::Input::_mouse.buttons[button] != action)
	{
		Voxel::Input::_mouse.buttons[button] = action;
		Voxel::Input::_mouse.lastButton = button;

		if (action)
		{
			Voxel::EventBus::Post(new Voxel::MouseButtonPressedEvent(button));
		}
		else
		{
			Voxel::EventBus::Post(new Voxel::MouseButtonReleasedEvent(button));
		}
	}
}

void cursor_callback(GLFWwindow* window, double x, double y)
{
	if (x != Voxel::Input::_mouse.position.x || y != Voxel::Input::_mouse.position.y)
	{
		Voxel::Input::_mouse.lastPosition = Voxel::Input::_mouse.position;
		Voxel::Input::_mouse.position = glm::vec2(x, y);

		Voxel::EventBus::Post(new Voxel::MouseMovedEvent(Voxel::Input::_mouse.position, Voxel::Input::_mouse.lastPosition));
	}
}

namespace Voxel
{

	Keyboard Input::_keyboard;
	Mouse Input::_mouse;
	GLFWwindow* Input::window;

	void Input::Init(GLFWwindow* window)
	{
		Input::window = window;
		
		glfwSetKeyCallback(window, key_callback);
		glfwSetMouseButtonCallback(window, mouse_callback);
		glfwSetCursorPosCallback(window, cursor_callback);

		SetMouseGrabbed(false);
	}

	bool Input::IsKeyPressed(KeyCode key)
	{
		if (key <= Key::Last) 
		{
			return _keyboard.keys[key];
		}
		return false;
	}

	bool Input::IsMouseButtonPressed(MouseButtonCode button)
	{
		if (button <= MouseButton::Last) 
		{
			return _mouse.buttons[button];
		}
		return false;
	}

	glm::vec2 Input::GetMousePosition()
	{
		return _mouse.position;
	}

	glm::vec2 Input::GetMouseLastPosition()
	{
		return _mouse.lastPosition;
	}

	void Input::SetMouseGrabbed(bool grabbed)
	{
		_mouse.grabbed = grabbed;
		glfwSetInputMode(Input::window, GLFW_CURSOR, grabbed ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
	}

	bool Input::GetMouseGrabbed()
	{
		return _mouse.grabbed;
	}

} // namespace Voxel