#pragma once

// Headers
#include <Config.h>
#include <string>
#include <functional>
#include <map>
#include <vector>

#define EVENT_TYPE(TYPE) virtual EventType GetType() const override { return TYPE; }\
	static EventType StaticType() { return TYPE; }

#define EVENT(TYPE) ((TYPE*) event)

#define CALLBACK(FUNC, OBJ) std::bind(&FUNC, OBJ, std::placeholders::_1)

namespace Voxel
{

	using EventType = std::string;

	class Event
	{
	public:
		virtual ~Event() = default;
		virtual EventType GetType() const = 0;

		bool IsHandled() const { return handled; }
		void SetHandled() { handled = true; }
	private:
		bool handled = false;
	};

	using EventCallback = std::function<void(const Event* event)>;

	class EventBus
	{
	public:
		template <typename E>
		static void Subscribe(const EventCallback& callback)
		{
			static_assert(std::is_base_of<Event, E>::value == true, "Subscribable object must derive from Event");
			_callbacks[E::StaticType()].push_back(callback);
		}

		static void Post(Event* event);
	private:
		static std::map<EventType, std::vector<EventCallback>> _callbacks;
	};

} // namespace Voxel