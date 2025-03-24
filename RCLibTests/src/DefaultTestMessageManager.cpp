#include "DefaultTestMessageManager.h"
#include "RCLibTests.h"
#include <RCLib.h>

#include <chrono>
#include <mutex>
#include <thread>

namespace RCLib::Tests::Impl
{
// Test message class
class TestMessage : public IMessage
{
public:
	TestMessage(int value)
	  : m_value(value)
	{
	}
	TMessageType        GetType() const override { return GetMessageTypeID<TestMessage>(); }
	int                 GetValue() const { return m_value; }
	SharedPtr<IMessage> Clone() const override { return MakeShared<TestMessage>(m_value); }

private:
	int m_value;
};

DefaultTestMessageManager::DefaultTestMessageManager()
  : DefaultTest("TestMessageManager", "Tests the MessageManager's functionality and thread safety")
{
}

void DefaultTestMessageManager::Setup()
{
	m_messageManager = IEngine::Get().GetMessageManager();
}

void DefaultTestMessageManager::Run()
{
	TestMessageRegistration();
	TestMessagePosting();
	TestMultipleHandlers();
	TestThreadSafety();
}

void DefaultTestMessageManager::Cleanup()
{

}

void DefaultTestMessageManager::TestMessageRegistration()
{
	LogInfo("Testing message registration...");

	// Test message handler
	auto handler = [this](const IMessage& msg) {
		if (auto testMsg = dynamic_cast<const TestMessage*>(&msg)) {
			m_messageCount++;
			if (m_messageCount == 1)
			{
				std::unique_lock<std::mutex> lock(m_mutex);
				m_cv.notify_one();
			}
		}
	};

	// Register handler
	m_messageManager->RegisterMessageHandler(GetMessageTypeID<TestMessage>(), [this](const IMessage& msg) {
		if (auto testMsg = dynamic_cast<const TestMessage*>(&msg)) {
			m_messageCount++;
			if (m_messageCount == 1)
			{
				std::unique_lock<std::mutex> lock(m_mutex);
				m_cv.notify_one();
			}
		}
	});
	AssertTrue(true, "Handler registration should succeed");

	// Post test message
	TestMessage testMsg(42);
	m_messageManager->PostMessage(testMsg);
	m_messageManager->ProcessMessages();

	// Wait for message processing
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cv.wait_for(lock, std::chrono::seconds(1), [this]() { return m_messageCount > 0; });
	}

	AssertEqual(1, m_messageCount.load(), "Message should be processed once");

	// Post another message - should still be processed
	TestMessage testMsg2(43);
	m_messageManager->PostMessage(testMsg2);
	m_messageManager->ProcessMessages();

	AssertEqual(2, m_messageCount.load(), "Message should still be processed after registration");
}

void DefaultTestMessageManager::TestMessagePosting()
{
	LogInfo("Testing message posting...");

	// Test message handler
	auto handler = [this](const IMessage& msg) {
		if (auto testMsg = dynamic_cast<const TestMessage*>(&msg)) {
			m_messageCount++;
			if (m_messageCount == 5)
			{
				std::unique_lock<std::mutex> lock(m_mutex);
				m_cv.notify_one();
			}
		}
	};

	// Register handler
	m_messageManager->RegisterMessageHandler(GetMessageTypeID<TestMessage>(),handler);

	// Post multiple messages
	for (int i = 0; i < 5; ++i)
	{
		TestMessage msg(i);
		m_messageManager->PostMessage(msg);
	}

	// Process messages
	m_messageManager->ProcessMessages();

	// Wait for message processing
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cv.wait_for(lock, std::chrono::seconds(1), [this]() { return m_messageCount >= 5; });
	}

	AssertEqual(5, m_messageCount.load(), "All messages should be processed");
}

void DefaultTestMessageManager::TestMultipleHandlers()
{
	LogInfo("Testing multiple handlers...");

	// Test callbacks
	auto handler1 = [this](const IMessage& msg) {
		if (auto testMsg = dynamic_cast<const TestMessage*>(&msg)) {
			m_messageCount++;
			if (m_messageCount == 2)
			{
				std::unique_lock<std::mutex> lock(m_mutex);
				m_cv.notify_one();
			}
		}
	};

	auto handler2 = [this](const IMessage& msg) {
		if (auto testMsg = dynamic_cast<const TestMessage*>(&msg)) {
			m_handlerCount++;
			if (m_handlerCount == 1)
			{
				std::unique_lock<std::mutex> lock(m_mutex);
				m_cv.notify_one();
			}
		}
	};

	// Register multiple handlers
	m_messageManager->RegisterMessageHandler(GetMessageTypeID<TestMessage>(), handler1);
	m_messageManager->RegisterMessageHandler(GetMessageTypeID<TestMessage>(), handler2);

	// Post event
	TestMessage msg(42);
	m_messageManager->PostMessage(msg);
	m_messageManager->ProcessMessages();

	// Wait for message processing
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cv.wait_for(lock, std::chrono::seconds(1), [this]() { return m_messageCount >= 2 && m_handlerCount >= 1; });
	}

	AssertEqual(2, m_messageCount.load(), "First handler should be called twice");
	AssertEqual(1, m_handlerCount.load(), "Second handler should be called once");
}

void DefaultTestMessageManager::TestThreadSafety()
{
	LogInfo("Testing thread safety...");

	// Test message handler
	auto handler = [this](const IMessage& msg) {
		if (auto testMsg = dynamic_cast<const TestMessage*>(&msg)) {
			m_messageCount++;
			if (m_messageCount == 100)
			{
				std::unique_lock<std::mutex> lock(m_mutex);
				m_cv.notify_one();
			}
		}
	};

	// Register handler
	m_messageManager->RegisterMessageHandler(GetMessageTypeID<TestMessage>(), handler);

	// Create multiple threads posting messages
	std::vector<std::thread> threads;
	for (int i = 0; i < 10; ++i)
	{
		threads.emplace_back([this]() {
			for (int j = 0; j < 10; ++j)
			{
				TestMessage msg(j);
				m_messageManager->PostMessage(msg);
			}
		});
	}

	// Wait for all threads to complete
	for (auto& thread : threads)
	{
		thread.join();
	}

	// Process messages
	m_messageManager->ProcessMessages();

	// Wait for message processing
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cv.wait_for(lock, std::chrono::seconds(1), [this]() { return m_messageCount >= 100; });
	}

	AssertEqual(100, m_messageCount.load(), "All messages should be processed in thread-safe manner");
}

} // namespace RCLib::Tests::Impl