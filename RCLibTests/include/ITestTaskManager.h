#pragma once
#include "ITest.h"
#include "RCLibTests_Fwd.h"

namespace RCLib::Tests
{

/**
 * @brief Interface for task manager tests
 */
class RCLIB_TESTS_API ITestTaskManager : public ITest
{
public:
	virtual ~ITestTaskManager() = default;

protected:
	// Test cases
	virtual void TestTaskSubmission()   = 0;
	virtual void TestTaskExecution()    = 0;
	virtual void TestTaskCancellation() = 0;
	virtual void TestThreadSafety()     = 0;

	ITestTaskManager() = default;
};

} // namespace RCLib::Tests