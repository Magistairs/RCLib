#include "EventSystem.h"
#include "Action.h"
#include "ActionManager.h"

#include <string>

using namespace RCLib;

void Event::Invoke()
{
	std::shared_lock<std::shared_mutex> lock(m_mutex);
	for (auto& func : m_callbacks)
	{
		func.second();
	}
}

void EventSystem::ProcessEvents()
{
	std::lock_guard<std::shared_mutex> lock(m_mutexQueuedEvents);
	while (!m_queuedEvents.empty())
	{
		int eventID = m_queuedEvents.front();
		m_queuedEvents.erase(m_queuedEvents.begin());

		std::shared_lock<std::shared_mutex> lock(m_mutexQueuedEventsCallbacks);
		m_queuedEventsCallbacks[eventID].Invoke();

		if (s_logEnabled)
		{
			std::string message = "[EventSystem] Event ID = " + std::to_string(eventID) + " invoked ("
				+ std::to_string(m_queuedEventsCallbacks[eventID].m_callbacks.size()) + " callbacks).";
			Logger::Get().Log(Logger::eInfo, message);
		}
	}
}

void EventSystem::QueueEvent(int eventID)
{
	std::shared_lock<std::shared_mutex> lock(m_mutexQueuedEventsCallbacks);

	auto it = m_queuedEventsCallbacks.find(eventID);

	if (it != m_queuedEventsCallbacks.end())
	{
		std::lock_guard<std::shared_mutex> lock(m_mutexQueuedEvents);
		for (int i : m_queuedEvents)
		{
			if (i == eventID)
				return;
		}
		m_queuedEvents.push_back(it->first);

		if (s_logEnabled)
		{
			Logger::Get().Log(Logger::eInfo, "[EventSystem] Event ID = " + std::to_string(eventID) + " queued.");
		}
	}
}

void EventSystem::EnableLog(bool value)
{
	s_logEnabled = value;
}