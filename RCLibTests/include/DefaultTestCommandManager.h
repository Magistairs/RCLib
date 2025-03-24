#pragma once

#include "ITestCommandManager.h"
#include "RCLibTests_Fwd.h"
#include "DefaultTest.h"
namespace RCLib::Tests::Impl
{

/**
 * @brief Implementation of the CommandManager test
 * @ingroup Tests
 */
class RCLIB_TESTS_API DefaultTestCommandManager : public DefaultTest,public ITestCommandManager
{
public:
	DefaultTestCommandManager();
	~DefaultTestCommandManager() override = default;

	// ITest interface
	void             Setup() override;
	void             Run() override;
	void             Cleanup() override;
	std::string_view GetName() const override { return "CommandManagerTest"; }
	std::string_view GetDescription() const override { return "Tests the CommandManager functionality"; }

protected:
	// Test cases
	void TestCommandCreation() override;
	void TestCommandExecution() override;
	void TestCommandQueue() override;
	void TestCommandUndo() override;
	void TestCommandExecuteLater() override;
	void TestCommandShortcuts() override;
	void TestCommandDescriptions() override;
	void TestCommandManagerPersistence() override;

};

} // namespace RCLib::Tests::Impl