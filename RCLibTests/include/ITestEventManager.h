#pragma once
#include "RCLibTests_Fwd.h"

namespace RCLib
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
class RCLIBTESTS_API ITestEventManager
{
public:
	virtual ~ITestEventManager() = default;

protected:
	ITestEventManager() = default;
};

} // namespace RCLib