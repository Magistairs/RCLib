#pragma once
#include "RCLib_Fwd.h"

namespace RCLib::Tests
{

/**
 * @brief Interface for task manager tests
 */
class RCLIB_API ITestTaskManager
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