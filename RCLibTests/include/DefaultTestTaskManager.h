#pragma once

#include "ITestTaskManager.h"
#include "RCLibTests_Fwd.h"
#include "DefaultTest.h"

#include <condition_variable>
#include <mutex>
#include <string>
#include <string_view>

namespace RCLib::Tests::Impl
{

/**
 * @brief Implementation of the TaskManager test
 * @ingroup Tests
 */
class RCLIB_TESTS_API DefaultTestTaskManager : public DefaultTest, public ITestTaskManager
{
public:
	DefaultTestTaskManager();
	~DefaultTestTaskManager() override = default;

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
	int                     m_taskCount{0};
	int                     m_completedTasks{0};
	std::mutex              m_mutex;
	std::condition_variable m_cv;
};

} // namespace RCLib::Tests::Impl