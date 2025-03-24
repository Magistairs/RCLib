#include "DefaultTestEventManager.h"
#include "RCLibTests.h"

#include <chrono>
#include <mutex>
#include <thread>

namespace RCLib::Tests::Impl
{

DefaultTestEventManager::DefaultTestEventManager()
  : DefaultTest("TestEventManager", "Tests the EventManager's functionality and thread safety")
{
}

void DefaultTestEventManager::Setup()
{
	m_eventCount    = 0;
	m_callbackCount = 0;
}

void DefaultTestEventManager::Run()
{
	TestEventRegistration();
	TestEventPosting();
	TestMultipleListeners();
	TestThreadSafety();
}

void DefaultTestEventManager::Cleanup() {}

void DefaultTestEventManager::TestEventRegistration()
{
	LogInfo("Testing event registration...");

	// Test event callback
	auto callback = [this]() {
		m_eventCount++;
		if (m_eventCount == 1)
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_cv.notify_one();
		}
	};

	// Register callback
	IEngine::Get().GetEventManager()->AddEventListener(this, 0, callback);
	AssertTrue(true, "Callback registration should succeed");

	// Post test event
	IEngine::Get().GetEventManager()->PostEvent(0);
	IEngine::Get().GetEventManager()->ProcessEvents();

	// Wait for event processing
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cv.wait_for(lock, std::chrono::seconds(1), [this]() { return m_eventCount > 0; });
	}

	AssertEqual(1, m_eventCount.load(), "Event should be processed once");

	// Remove callback
	IEngine::Get().GetEventManager()->RemoveEventListener(this, 0);

	// Post another event - should not be processed
	IEngine::Get().GetEventManager()->PostEvent(0);
	IEngine::Get().GetEventManager()->ProcessEvents();

	AssertEqual(1, m_eventCount.load(), "Event should not be processed after removal");
}

void DefaultTestEventManager::TestEventPosting()
{
	LogInfo("Testing event posting...");

	// Test event callback
	auto callback = [this]() {
		m_eventCount++;
		if (m_eventCount == 5)
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_cv.notify_one();
		}
	};

	// Register callback
	IEngine::Get().GetEventManager()->AddEventListener(this, 0, callback);

	// Post multiple events
	for (int i = 0; i < 5; ++i)
	{
		IEngine::Get().GetEventManager()->PostEvent(0);
	}

	// Process events
	IEngine::Get().GetEventManager()->ProcessEvents();

	// Wait for event processing
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cv.wait_for(lock, std::chrono::seconds(1), [this]() { return m_eventCount >= 5; });
	}

	AssertEqual(5, m_eventCount.load(), "All events should be processed");

	// Cleanup
	IEngine::Get().GetEventManager()->RemoveEventListener(this, 0);
}

void DefaultTestEventManager::TestMultipleListeners()
{
	LogInfo("Testing multiple listeners...");

	// Test callbacks
	auto callback1 = [this]() {
		m_eventCount++;
		if (m_eventCount == 2)
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_cv.notify_one();
		}
	};

	auto callback2 = [this]() {
		m_callbackCount++;
		if (m_callbackCount == 1)
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_cv.notify_one();
		}
	};

	// Register multiple callbacks
	IEngine::Get().GetEventManager()->AddEventListener(this, 0, callback1);
	IEngine::Get().GetEventManager()->AddEventListener(this, 0, callback2);

	// Post event
	IEngine::Get().GetEventManager()->PostEvent(0);
	IEngine::Get().GetEventManager()->ProcessEvents();

	// Wait for event processing
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cv.wait_for(lock, std::chrono::seconds(1), [this]() { return m_eventCount >= 2 && m_callbackCount >= 1; });
	}

	AssertEqual(2, m_eventCount.load(), "First callback should be called twice");
	AssertEqual(1, m_callbackCount.load(), "Second callback should be called once");

	// Cleanup
	IEngine::Get().GetEventManager()->RemoveEventListener(this, 0);
}

void DefaultTestEventManager::TestThreadSafety()
{
	LogInfo("Testing thread safety...");

	// Test event callback
	auto callback = [this]() {
		m_eventCount++;
		if (m_eventCount == 100)
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_cv.notify_one();
		}
	};

	// Register handler
	IEngine::Get().GetEventManager()->AddEventListener(this, 0, callback);

	// Create multiple threads posting events
	std::vector<std::thread> threads;
	for (int i = 0; i < 10; ++i)
	{
		threads.emplace_back([this]() {
			for (int j = 0; j < 10; ++j)
			{
				IEngine::Get().GetEventManager()->PostEvent(0);
			}
		});
	}

	// Wait for all threads to complete
	for (auto& thread : threads)
	{
		thread.join();
	}

	// Process events
	IEngine::Get().GetEventManager()->ProcessEvents();

	// Wait for event processing
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cv.wait_for(lock, std::chrono::seconds(1), [this]() { return m_eventCount >= 100; });
	}

	AssertEqual(100, m_eventCount.load(), "All messages should be processed in thread-safe manner");
}

} // namespace RCLib::Tests::Impl