#pragma once

#include "RCLibTests_Fwd.h"
#include "DLLExport.h"
#include "ITestMessageManager.h"
#include "DefaultTest.h"
#include "DefaultMessageManager.h"
#include "IMessageManager.h"

#include <mutex>
#include <condition_variable>
#include <atomic>
#include <string>
#include <string_view>
#include <memory>

namespace RCLib::Tests::Impl
{

/**
 * @brief Implementation of the MessageManager test
 * @ingroup Tests
 */
class RCLIBTESTS_API TestMessageManagerImpl
  : public DefaultTest
  , public ITestMessageManager
{
public:
	TestMessageManagerImpl();
	~TestMessageManagerImpl() override = default;

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
	std::shared_ptr<RCLib::Impl::DefaultMessageManager> m_messageManager;
	std::atomic<int>                                    m_messageCount{0};
	std::atomic<int>                                    m_handlerCount{0};
	std::mutex                                          m_mutex;
	std::condition_variable                             m_cv;
};

} // namespace RCLib::Tests::Impl