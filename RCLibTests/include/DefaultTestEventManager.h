#pragma once
#include "RCLibTests_Fwd.h"
#include "IEventManager.h"
#include "ITestEventManager.h"
#include "DefaultTest.h"
#include "DefaultEventManager.h"
#include <mutex>
#include <condition_variable>
#include <atomic>

namespace RCLib::Tests::Impl
{
/**
 * @brief Implementation of the EventManager test
 */
class RCLIB_API TestEventManagerImpl
  : public ITestEventManager
  , public DefaultTest
{
public:
	TestEventManagerImpl();
	~TestEventManagerImpl() override = default;

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
	SharedPtr<DefaultMessageManager> m_messageManager;
	std::mutex                      m_mutex;
	std::condition_variable         m_cv;
	int                            m_messageCount{0};
	int                            m_handlerCount{0};
};

} // namespace RCLib::Tests::Impl