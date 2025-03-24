#pragma once
#include "DefaultTest.h"
#include "ITestEventManager.h"
#include "RCLibTests_Fwd.h"

#include <atomic>
#include <condition_variable>
#include <mutex>

namespace RCLib::Tests::Impl
{
/**
 * @brief Implementation of the EventManager test
 */
class RCLIB_TESTS_API DefaultTestEventManager
  : public DefaultTest
  , public ITestEventManager
{
public:
	DefaultTestEventManager();
	~DefaultTestEventManager() override = default;

	// ITestEventManager interface implementation
	void             Setup() override;
	void             Run() override;
	void             Cleanup() override;
	std::string_view GetName() const override { return "TestEventManager"; }
	std::string_view GetDescription() const override { return "Tests the EventManager's functionality and thread safety"; }

protected:
	void TestEventRegistration();
	void TestEventPosting();
	void TestMultipleListeners();
	void TestThreadSafety();

private:
	std::mutex               m_mutex;
	std::condition_variable  m_cv;
	std::atomic<int>         m_eventCount{0};
	std::atomic<int>         m_callbackCount{0};
	SharedPtr<IEventManager> m_eventManager;
};

} // namespace RCLib::Tests::Impl