#pragma once
#include "RCLibTests_Fwd.h"

namespace RCLib::Tests
{
class RCLIBTESTS_API ITestCommandManager
{
public:
	virtual ~ITestCommandManager() = default;

protected:
	// Test cases
	virtual void TestCommandCreation()           = 0;
	virtual void TestCommandExecution()          = 0;
	virtual void TestCommandQueue()              = 0;
	virtual void TestCommandUndo()               = 0;
	virtual void TestCommandExecuteLater()       = 0;
	virtual void TestCommandShortcuts()          = 0;
	virtual void TestCommandDescriptions()       = 0;
	virtual void TestCommandManagerPersistence() = 0;

	ITestCommandManager() = default;
};
} // namespace RCLib::Tests