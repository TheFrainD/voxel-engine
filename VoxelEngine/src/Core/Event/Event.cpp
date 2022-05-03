#include <vepch.h>
#include "Event.h"

// Headers
#include <Config.h>
#include <string>
#include <functional>
#include <map>
#include <vector>
#include <algorithm>

namespace Voxel
{

	std::map<EventType, std::vector<EventCallback>> EventBus::_callbacks;

	void EventBus::Post(Event* event)
	{
		if (_callbacks.find(event->GetType()) != _callbacks.end())
		{
			for (auto&& callback : _callbacks.at(event->GetType()))
			{
				if (!event->IsHandled())
				{
					callback(event);
				}
			}
		}

		event->SetHandled();
		delete event;
	}

} // namespace Voxel