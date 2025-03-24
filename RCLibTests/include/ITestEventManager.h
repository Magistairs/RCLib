#pragma once
#include "ITest.h"
#include "RCLibTests_Fwd.h"

namespace RCLib::Tests
{
/**
 * @brief Interface for testing the EventManager
 *
 * Tests the following functionality:
 * - Event listener registration and removal
 * - Event posting and processing
 * - Multiple listeners for same event
 * - Thread safety
 */
class RCLIB_TESTS_API ITestEventManager : public ITest
{
public:
	virtual ~ITestEventManager() = default;

protected:
	ITestEventManager() = default;
};

} // namespace RCLib::Tests