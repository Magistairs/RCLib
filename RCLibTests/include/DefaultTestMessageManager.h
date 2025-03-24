#pragma once

#include "DefaultTest.h"
#include "ITestMessageManager.h"
#include "RCLibTests_Fwd.h"

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <string>
#include <string_view>

namespace RCLib::Tests::Impl
{

/**
 * @brief Implementation of the MessageManager test
 * @ingroup Tests
 */
class RCLIB_TESTS_API DefaultTestMessageManager : public DefaultTest, public ITestMessageManager
{
public:
	DefaultTestMessageManager();
	~DefaultTestMessageManager() override = default;

	// ITestMessageManager interface implementation
	void             Setup() override;
	void             Run() override;
	void             Cleanup() override;
	std::string_view GetName() const override { return "TestMessageManager"; }
	std::string_view GetDescription() const override { return "Tests the MessageManager's functionality and thread safety"; }

protected:
	void TestMessageRegistration();
	void TestMessagePosting();
	void TestMultipleHandlers();
	void TestThreadSafety();

private:
	std::atomic<int>        m_messageCount{0};
	std::atomic<int>        m_handlerCount{0};
	std::mutex              m_mutex;
	std::condition_variable m_cv;
	SharedPtr<IMessageManager> m_messageManager;
};

} // namespace RCLib::Tests::Impl