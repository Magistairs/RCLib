#pragma once
#include "RCLibTests_Fwd.h"

namespace RCLib::Tests
{

/**
 * @brief Interface for testing the MessageManager
 *
 * Tests the following functionality:
 * - Message handler registration and removal
 * - Message posting and processing
 * - Multiple handlers for same message type
 * - Thread safety
 */
class RCLIBTESTS_API ITestMessageManager
{
public:
	virtual ~ITestMessageManager() = default;

protected:
	ITestMessageManager() = default;
};

} // namespace RCLib::Tests