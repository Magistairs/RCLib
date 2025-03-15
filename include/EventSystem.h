#pragma once
#include "Logger.h"
#include "DLLExport.h"

#include <deque>
#include <functional>
#include <iostream>
#include <shared_mutex>
#include <string>
#include <unordered_map>
#include <vector>

namespace RCLib
{
	class Event
	{
	public:
		Event() = default;
		~Event() = default;

		Event(Event&&) = delete;
		Event(const Event&) = delete;
		Event& operator=(Event&&) = delete;
		Event& operator=(const Event&) = delete;

	private:
		friend class EventSystem;
		using TCallBackPair = std::pair<void*, std::function<void()>>;
		template<typename TCallBackOwner>
		void AddCallback(TCallBackOwner* pCallBackOwner, std::function<void()> func)
		{
			std::lock_guard<std::shared_mutex> lock(m_mutex);

			TCallBackPair cb = { (void*)pCallBackOwner, func };
			m_callbacks.push_back(std::move(cb));
		}

		template<typename TCallBackOwner>
		bool RemoveCallback(TCallBackOwner* pCallBackOwner)
		{
			std::lock_guard<std::shared_mutex> lock(m_mutex);

			size_t initialSize = m_callbacks.size();

			m_callbacks.erase(
				std::remove_if(m_callbacks.begin(), m_callbacks.end(), [pCallBackOwner](TCallBackPair& pair) { return (pair.first == pCallBackOwner); }),
				m_callbacks.end());

			return m_callbacks.size() < initialSize;
		}

		void Invoke();

		std::vector<TCallBackPair> m_callbacks;
		std::shared_mutex          m_mutex;
	};

#define UNIQUE_EVENT_ID inline static int ID = EventSystem::s_callBackID++;
	// How to declare an unique ID for each callbacks with args:
	//	class NewEvent
	//	{
	// 	public:
	//		UNIQUE_EVENT_ID
	// 		...
	//	};

	class RCLIB_API EventSystem
	{
		struct Callback
		{
			void* pOwnerAdress;
			int   eventID; // Unique ID for each Event
		};
		using TCallBackPair = std::pair<Callback, std::function<void(void*)>>;

	public:
		inline static int s_callBackID = 0;

		EventSystem() = default;
		~EventSystem() = default;

		EventSystem(EventSystem&&) = delete;
		EventSystem(const EventSystem&) = delete;
		EventSystem& operator=(EventSystem&&) = delete;
		EventSystem& operator=(const EventSystem&) = delete;

		template<typename TCallBackOwner>
		void AddQueuedEventCallback(TCallBackOwner* pCallBackOwner, int eventID, std::function<void()> func)
		{
			std::shared_lock<std::shared_mutex> lock(m_mutexQueuedEventsCallbacks);

			m_queuedEventsCallbacks[eventID].AddCallback(pCallBackOwner, func);

			if (s_logEnabled)
			{
				Logger::Get().Log(Logger::eInfo,
					"[EventSystem] Event ID = " + std::to_string(eventID) + ": callback added by " + typeid(TCallBackOwner).name());
			}
		}

		template<typename TCallBackOwner>
		void RemoveQueuedEventCallback(TCallBackOwner* pCallBackOwner, int eventID)
		{
			std::shared_lock<std::shared_mutex> lock(m_mutexQueuedEventsCallbacks);

			auto it = m_queuedEventsCallbacks.find(eventID);
			if (it != m_queuedEventsCallbacks.end())
			{
				if (m_queuedEventsCallbacks[eventID].RemoveCallback(pCallBackOwner))
				{
					if (s_logEnabled)
					{
						Logger::Get().Log(Logger::eInfo,
							"[EventSystem] Event ID = " + std::to_string(eventID) + ": callback removed by " + typeid(TCallBackOwner).name());
					}
				}
				else
				{
					Logger::Get().Log(Logger::eWarning,
						"[EventSystem] Event ID = " + std::to_string(eventID) + ": callback asked for removing by " + typeid(TCallBackOwner).name()
						+ " but was not found!");
				}
			}
		}

		// Push an event to the queue that will be processed later
		void QueueEvent(int eventID);

		// Process events in queue
		void ProcessEvents();

		// To add a call back in the event system using the adress of the call back owner and a function with args
		template<typename TEvent, typename TCallBackOwner>
		void AddCallback(TCallBackOwner* pCallBackOwner, std::function<void(const TEvent&)>&& func)
		{
			std::lock_guard<std::shared_mutex> lock(m_mutexEventsCallback);

			Callback cb = { (void*)pCallBackOwner, TEvent::ID };

			TCallBackPair pair(std::move(cb), [func](void* pEventObject) { func(*(const TEvent*)pEventObject); });

			m_eventsCallback.push_back(std::move(pair));
			if (s_logEnabled)
			{
				Logger::Get().Log(Logger::eInfo,
					"[EventSystem] Event ID = " + std::to_string(TEvent::ID) + ": callback added by " + typeid(TCallBackOwner).name());
			}
		}

		// To remove a call back in the event system using the adress of the call back owner
		template<typename TEvent, typename TCallBackOwner>
		void RemoveCallback(TCallBackOwner* pCallBackOwner)
		{
			std::lock_guard<std::shared_mutex> lock(m_mutexEventsCallback);

			int        eventID = 0;
			auto       it = std::remove_if(m_eventsCallback.begin(), m_eventsCallback.end(), [pCallBackOwner, &eventID](TCallBackPair& pair) {
				if (pair.first.eventID == TEvent::ID && pair.first.pOwnerAdress == pCallBackOwner)
				{
					eventID = pair.first.eventID;
					return true;
				}
				else
					return false;
				});
			const bool found = it != m_eventsCallback.end();

			if (found)
			{
				m_eventsCallback.erase(it);
				if (s_logEnabled)
				{
					Logger::Get().Log(Logger::eInfo,
						"[EventSystem] Event ID = " + std::to_string(eventID) + ": callback removed by " + typeid(TCallBackOwner).name());
				}
			}

			else
			{
				Logger::Get().Log(Logger::eWarning,
					"[EventSystem] Event ID = " + std::to_string(eventID) + ": callback asked for removing by " + typeid(TCallBackOwner).name()
					+ " but was not found!");
			}
		}

		// To call an event
		template<typename TEvent>
		void SendEvent(const TEvent& typeEvent)
		{
			std::shared_lock<std::shared_mutex> lock(m_mutexEventsCallback);

			for (auto& cb : m_eventsCallback)
			{
				if (cb.first.eventID == TEvent::ID)
				{
					cb.second((void*)&typeEvent);
				}
			}
			if (s_logEnabled)
			{
				std::string message =
					"[EventSystem] Event ID = " + std::to_string(TEvent::ID) + " invoked (" + std::to_string(m_eventsCallback.size()) + " callbacks).";
				Logger::Get().Log(Logger::eInfo, message);
			}
		}

		static void EnableLog(bool value);

	private:
		std::vector<TCallBackPair>     m_eventsCallback;
		std::unordered_map<int, Event> m_queuedEventsCallbacks;
		std::deque<int>                m_queuedEvents;

		std::shared_mutex m_mutexEventsCallback;
		std::shared_mutex m_mutexQueuedEventsCallbacks;
		std::shared_mutex m_mutexQueuedEvents;

		inline static std::atomic_bool s_logEnabled{ false };
	};
} // namespace RCLib