#include "DefaultEventManager.h"
#include "RCLib.h"

namespace RCLib::Impl
{

DefaultEventManager::DefaultEventManager() = default;

bool DefaultEventManager::OnInitialize()
{
	try
	{
		IEngine::Get().GetLogger()->Info("Event manager initialized");
		return true;
	}
	catch (const std::exception& e)
	{
		IEngine::Get().GetLogger()->Error(std::string("Event manager initialization failed: ") + e.what());
		return false;
	}
}

void DefaultEventManager::OnRelease()
{
	try
	{
		std::unique_lock<std::shared_mutex> lock(m_eventListenersMutex);
		m_eventListeners.clear();
		m_pendingEvents.clear();
		IEngine::Get().GetLogger()->Info("Event manager released");
	}
	catch (const std::exception& e)
	{
		IEngine::Get().GetLogger()->Error(std::string("Event manager release failed: ") + e.what());
	}
}

void DefaultEventManager::AddEventListener(void* owner, TEventType eventID, FEventCallback callback)
{
	try
	{
		std::unique_lock<std::shared_mutex> lock(m_eventListenersMutex);
		m_eventListeners[eventID].push_back(std::make_pair(owner, callback));
		IEngine::Get().GetLogger()->Info(std::string("Event listener added for event: ") + std::to_string(eventID));
	}
	catch (const std::exception& e)
	{
		IEngine::Get().GetLogger()->Error(std::string("Failed to add event listener: ") + e.what());
	}
}

void DefaultEventManager::RemoveEventListener(void* owner, TEventType eventID)
{
	try
	{
		std::unique_lock<std::shared_mutex> lock(m_eventListenersMutex);
		auto                                it = m_eventListeners.find(eventID);
		if (it != m_eventListeners.end())
		{
			auto& listeners = it->second;
			listeners.erase(std::remove_if(listeners.begin(), listeners.end(), [owner](const auto& pair) { return pair.first == owner; }), listeners.end());

			if (listeners.empty())
			{
				m_eventListeners.erase(it);
			}
			IEngine::Get().GetLogger()->Info(std::string("Event listener removed for event: ") + std::to_string(eventID));
		}
	}
	catch (const std::exception& e)
	{
		IEngine::Get().GetLogger()->Error(std::string("Failed to remove event listener: ") + e.what());
	}
}

void DefaultEventManager::RemoveAllEventListeners(void* owner)
{
	try
	{
		std::unique_lock<std::shared_mutex> lock(m_eventListenersMutex);
		for (auto it = m_eventListeners.begin(); it != m_eventListeners.end();)
		{
			auto& listeners = it->second;
			listeners.erase(std::remove_if(listeners.begin(), listeners.end(), [owner](const auto& pair) { return pair.first == owner; }), listeners.end());

			if (listeners.empty())
			{
				it = m_eventListeners.erase(it);
			}
			else
			{
				++it;
			}
		}
		IEngine::Get().GetLogger()->Info("All event listeners removed for owner");
	}
	catch (const std::exception& e)
	{
		IEngine::Get().GetLogger()->Error(std::string("Failed to remove all event listeners: ") + e.what());
	}
}

void DefaultEventManager::PostEvent(TEventType eventID)
{
	try
	{
		std::lock_guard<std::mutex> lock(m_pendingEventsMutex);
		m_pendingEvents.push_back(eventID);
		IEngine::Get().GetLogger()->Info(std::string("Event posted: ") + std::to_string(eventID));
	}
	catch (const std::exception& e)
	{
		IEngine::Get().GetLogger()->Error(std::string("Failed to post event: ") + e.what());
	}
}

void DefaultEventManager::ProcessEvents()
{
	try
	{
		std::vector<TEventType> currentEvents;
		{
			std::lock_guard<std::mutex> lock(m_pendingEventsMutex);
			currentEvents = std::move(m_pendingEvents);
		}

		for (const auto& eventID : currentEvents)
		{
			std::vector<std::pair<void*, FEventCallback>> listeners;
			{
				std::shared_lock<std::shared_mutex> lock(m_eventListenersMutex);
				auto                                it = m_eventListeners.find(eventID);
				if (it != m_eventListeners.end())
				{
					listeners = it->second;
				}
			}

			for (const auto& [owner, callback] : listeners)
			{
				try
				{
					callback();
				}
				catch (const std::exception& e)
				{
					IEngine::Get().GetLogger()->Error(std::string("Event callback failed: ") + e.what());
				}
			}
		}
	}
	catch (const std::exception& e)
	{
		IEngine::Get().GetLogger()->Error(std::string("Failed to process events: ") + e.what());
	}
}

bool DefaultEventManager::OnUpdate()
{
	try
	{
		ProcessEvents();
		return true;
	}
	catch (const std::exception& e)
	{
		IEngine::Get().GetLogger()->Error(std::string("Event manager update failed: ") + e.what());
		return false;
	}
}

} // namespace RCLib::Impl