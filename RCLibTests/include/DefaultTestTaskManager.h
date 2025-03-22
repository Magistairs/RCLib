#pragma once

#include "RCLibTests_Fwd.h"
#include "DLLExport.h"
#include "ITestTaskManager.h"
#include "DefaultTest.h"
#include "DefaultTaskManager.h"
#include "ITaskManager.h"

#include <mutex>
#include <condition_variable>
#include <string>
#include <string_view>
#include <memory>

namespace RCLib::Tests::Impl
{

/**
 * @brief Implementation of the TaskManager test
 * @ingroup Tests
 */
class RCLIBTESTS_API TestTaskManagerImpl
  : public DefaultTest
  , public ITestTaskManager
{
public:
	TestTaskManagerImpl();
	~TestTaskManagerImpl() override = default;

	// ITest interface implementation
	void             Setup() override;
	void             Run() override;
	void             Cleanup() override;
	std::string_view GetName() const override { return "TestTaskManager"; }
	std::string_view GetDescription() const override { return "Tests the TaskManager's functionality and thread safety"; }

protected:
	// Test cases
	void TestTaskSubmission() override;
	void TestTaskExecution() override;
	void TestTaskCancellation() override;
	void TestThreadSafety() override;

private:
	std::shared_ptr<RCLib::Impl::TaskManagerImpl> m_manager;
	int                                           m_taskCount{0};
	int                                           m_completedTasks{0};
	std::mutex                                    m_mutex;
	std::condition_variable                       m_cv;
};

} // namespace RCLib::Tests::Impl