#include "DefaultTestMessageManager.h"
#include "RCLibTests.h"
#include "DefaultEngine.h"
#include <mutex>
#include <thread>
#include <chrono>


namespace RCLib::Impl
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
TestMessageManagerImpl::TestMessageManagerImpl()
  : DefaultTest("TestMessageManager")
{
	m_messageCount = 0;
	m_handlerCount = 0;
}

void TestMessageManagerImpl::Setup()
{
	m_messageManager = MakeShared<DefaultMessageManager>();
	m_messageManager->Initialize();
}

void TestMessageManagerImpl::Run()
{
	TestMessageRegistration();
	TestMessagePosting();
	TestMultipleHandlers();
	TestThreadSafety();
}

void TestMessageManagerImpl::Cleanup()
{
	// Cleanup is handled in individual test methods
}

void TestMessageManagerImpl::TestMessageRegistration()
{
	LogInfo("Testing message registration...");

	// Test message handler
	auto handler = [this](const TestMessage& msg) {
		m_messageCount++;
		if (m_messageCount == 1)
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_cv.notify_one();
		}
	};

	// Register handler
	m_messageManager->RegisterMessageHandler<TestMessage>(handler);
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

	AssertEqual(1, m_messageCount, "Message should be processed once");

	// Post another message - should still be processed
	TestMessage testMsg2(43);
	m_messageManager->PostMessage(testMsg2);
	m_messageManager->ProcessMessages();

	AssertEqual(2, m_messageCount, "Message should still be processed after registration");
}

void TestMessageManagerImpl::TestMessagePosting()
{
	LogInfo("Testing message posting...");

	// Test message handler
	auto handler = [this](const TestMessage& msg) {
		m_messageCount++;
		if (m_messageCount == 5)
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_cv.notify_one();
		}
	};

	// Register handler
	m_messageManager->RegisterMessageHandler<TestMessage>(handler);

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

	AssertEqual(5, m_messageCount, "All messages should be processed");
}

void TestMessageManagerImpl::TestMultipleHandlers()
{
	LogInfo("Testing multiple handlers...");

	// Test handlers
	auto handler1 = [this](const TestMessage& msg) {
		m_messageCount++;
		if (m_messageCount == 2)
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_cv.notify_one();
		}
	};

	auto handler2 = [this](const TestMessage& msg) {
		m_handlerCount++;
		if (m_handlerCount == 1)
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_cv.notify_one();
		}
	};

	// Register multiple handlers
	m_messageManager->RegisterMessageHandler<TestMessage>(handler1);
	m_messageManager->RegisterMessageHandler<TestMessage>(handler2);

	// Post message
	TestMessage msg(42);
	m_messageManager->PostMessage(msg);
	m_messageManager->ProcessMessages();

	// Wait for message processing
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cv.wait_for(lock, std::chrono::seconds(1), [this]() { return m_messageCount >= 2 && m_handlerCount >= 1; });
	}

	AssertEqual(2, m_messageCount, "First handler should be called twice");
	AssertEqual(1, m_handlerCount, "Second handler should be called once");
}

void TestMessageManagerImpl::TestThreadSafety()
{
	LogInfo("Testing thread safety...");

	// Test message handler
	auto handler = [this](const TestMessage& msg) {
		m_messageCount++;
		if (m_messageCount == 100)
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_cv.notify_one();
		}
	};

	// Register handler
	m_messageManager->RegisterMessageHandler<TestMessage>(handler);

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

	AssertEqual(100, m_messageCount, "All messages should be processed in thread-safe manner");
}

} // namespace RCLib::Impl