#pragma once
#include "ITest.h"
#include "RCLibTests_Fwd.h"

namespace RCLib::Tests
{
/**
 * @brief Pure interface for engine testing
 */
class RCLIB_TESTS_API ITestEngine : public ITest
{
public:
	virtual ~ITestEngine() = default;

protected:
	ITestEngine() = default;

	// Test methods that can be overridden by derived classes
	virtual void TestManagerLifecycle()          = 0;
	virtual void TestCustomManagerRegistration() = 0;
	virtual void TestManagerUpdateOrder()        = 0;
	virtual void TestManagerExceptions()         = 0;
};
} // namespace RCLib::Tests