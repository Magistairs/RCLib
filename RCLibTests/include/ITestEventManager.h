#pragma once
#include "RCLib_Fwd.h"

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
class RCLIB_API ITestEventManager
{
public:
	virtual ~ITestEventManager() = default;

protected:
	ITestEventManager() = default;
};

} // namespace RCLib