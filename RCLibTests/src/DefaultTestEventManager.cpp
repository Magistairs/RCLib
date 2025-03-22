#include "DefaultTestEventManager.h"
#include "RCLibTests.h"
#include "DefaultEngine.h"
#include <mutex>
#include <thread>
#include <chrono>

namespace RCLib::Impl
{

TestEventManagerImpl::TestEventManagerImpl()
  : DefaultTest("TestEventManager")
{
}

void TestEventManagerImpl::Setup()
{
	m_eventCount    = 0;
	m_callbackCount = 0;
	m_eventManager  = MakeShared<EventManagerImpl>();
}

void TestEventManagerImpl::Run()
{
	TestEventRegistration();
	TestEventPosting();
	TestMultipleListeners();
	TestThreadSafety();
}

void TestEventManagerImpl::Cleanup()
{
	// Cleanup is handled in individual test methods
}

void TestEventManagerImpl::TestEventRegistration()
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
	m_eventManager->AddEventListener(this, 0, callback);
	AssertTrue(true, "Callback registration should succeed");

	// Post test event
	m_eventManager->PostEvent(0);
	m_eventManager->ProcessEvents();

	// Wait for event processing
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cv.wait_for(lock, std::chrono::seconds(1), [this]() { return m_eventCount > 0; });
	}

	AssertEqual(1, m_eventCount, "Event should be processed once");

	// Remove callback
	m_eventManager->RemoveEventListener(this, 0);

	// Post another event - should not be processed
	m_eventManager->PostEvent(0);
	m_eventManager->ProcessEvents();

	AssertEqual(1, m_eventCount, "Event should not be processed after removal");
}

void TestEventManagerImpl::TestEventPosting()
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
	m_eventManager->AddEventListener(this, 0, callback);

	// Post multiple events
	for (int i = 0; i < 5; ++i)
	{
		m_eventManager->PostEvent(0);
	}

	// Process events
	m_eventManager->ProcessEvents();

	// Wait for event processing
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cv.wait_for(lock, std::chrono::seconds(1), [this]() { return m_eventCount >= 5; });
	}

	AssertEqual(5, m_eventCount, "All events should be processed");

	// Cleanup
	m_eventManager->RemoveEventListener(this, 0);
}

void TestEventManagerImpl::TestMultipleListeners()
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
	m_eventManager->AddEventListener(this, 0, callback1);
	m_eventManager->AddEventListener(this, 0, callback2);

	// Post event
	m_eventManager->PostEvent(0);
	m_eventManager->ProcessEvents();

	// Wait for event processing
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cv.wait_for(lock, std::chrono::seconds(1), [this]() { return m_eventCount >= 2 && m_callbackCount >= 1; });
	}

	AssertEqual(2, m_eventCount, "First callback should be called twice");
	AssertEqual(1, m_callbackCount, "Second callback should be called once");

	// Cleanup
	m_eventManager->RemoveEventListener(this, 0);
}

void TestEventManagerImpl::TestThreadSafety()
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

	// Register callback
	m_eventManager->AddEventListener(this, 0, callback);

	// Create multiple threads posting events
	std::vector<std::thread> threads;
	for (int i = 0; i < 10; ++i)
	{
		threads.emplace_back([this]() {
			for (int j = 0; j < 10; ++j)
			{
				m_eventManager->PostEvent(0);
			}
		});
	}

	// Wait for all threads to complete
	for (auto& thread : threads)
	{
		thread.join();
	}

	// Process events
	m_eventManager->ProcessEvents();

	// Wait for event processing
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cv.wait_for(lock, std::chrono::seconds(1), [this]() { return m_eventCount >= 100; });
	}

	AssertEqual(100, m_eventCount, "All events should be processed in thread-safe manner");

	// Cleanup
	m_eventManager->RemoveEventListener(this, 0);
}

} // namespace RCLib::Impl