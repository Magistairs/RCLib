#pragma once
#include "RCLibTests_Fwd.h"

namespace RCLib::Tests
{

/**
 * @brief Interface for testing the memory manager
 */
class RCLIB_API ITestMemoryManager
{
public:
	virtual ~ITestMemoryManager() = default;

protected:
	// Test cases that derived classes must implement
	virtual void TestAllocation()    = 0;
	virtual void TestDeallocation()  = 0;
	virtual void TestTracking()      = 0;
	virtual void TestLeakDetection() = 0;
	virtual void TestThreadSafety()  = 0;

	ITestMemoryManager() = default;
};

} // namespace RCLib::Tests