#include "DefaultTestCommandManager.h"
#include "RCLibTests.h"

namespace RCLib::Tests::Impl
{
DefaultTestCommandManager::DefaultTestCommandManager()
  : DefaultTest("TestCommandManager")
{
}

void DefaultTestCommandManager::Setup()
{

}

void DefaultTestCommandManager::Run()
{
	RunTest("Command Creation", [this] { TestCommandCreation(); });
	RunTest("Command Execution", [this] { TestCommandExecution(); });
	RunTest("Command Queue", [this] { TestCommandQueue(); });
	RunTest("Command Undo", [this] { TestCommandUndo(); });
	RunTest("Command Execute Later", [this] { TestCommandExecuteLater(); });
	RunTest("Command Shortcuts", [this] { TestCommandShortcuts(); });
	RunTest("Command Descriptions", [this] { TestCommandDescriptions(); });
	RunTest("Command Manager Persistence", [this] { TestCommandManagerPersistence(); });
}

void DefaultTestCommandManager::Cleanup()
{

}

void DefaultTestCommandManager::TestCommandCreation()
{
	auto pCommand = IEngine::Get().GetCommandManager()->AddCommand("TestCommand");
	AssertNotNull(pCommand.get(), "Command should be created");
	AssertStringEqual(pCommand->GetName(), "TestCommand", "Command name should match");
}

void DefaultTestCommandManager::TestCommandExecution()
{
	bool executed = false;
	auto pCommand = IEngine::Get().GetCommandManager()->AddCommand("TestExecution");
	pCommand->SetCallback([&executed]() { executed = true; });
	pCommand->Execute();
	AssertTrue(executed, "Command execution failed");
}

void DefaultTestCommandManager::TestCommandQueue()
{
	size_t initialQueueSize = IEngine::Get().GetCommandManager()->GetQueueSize();
	auto   pCommand         = IEngine::Get().GetCommandManager()->AddCommand("TestQueue");
	pCommand->ExecuteLater();
	AssertEqual(IEngine::Get().GetCommandManager()->GetQueueSize(), initialQueueSize + 1, "Command queue size mismatch");
}

void DefaultTestCommandManager::TestCommandUndo()
{
	bool executed = false;
	bool undone   = false;
	auto pCommand = IEngine::Get().GetCommandManager()->AddCommand("TestUndo");
	pCommand->SetCallback([&executed]() { executed = true; });
	pCommand->SetUndoCallback([&undone]() { undone = true; });
	pCommand->Execute();
	AssertTrue(executed, "Command execution failed");
	IEngine::Get().GetCommandManager()->Undo();
	AssertTrue(undone, "Command undo failed");
}

void DefaultTestCommandManager::TestCommandExecuteLater()
{
	bool executed = false;
	auto pCommand = IEngine::Get().GetCommandManager()->AddCommand("TestExecuteLater");
	pCommand->SetCallback([&executed]() { executed = true; });
	pCommand->ExecuteLater();
	AssertFalse(executed, "Command executed immediately instead of being queued");
	IEngine::Get().GetCommandManager()->OnUpdate();
	AssertTrue(executed, "Command not executed after update");
}

void DefaultTestCommandManager::TestCommandShortcuts()
{
	auto pCommand = IEngine::Get().GetCommandManager()->AddCommand("TestShortcuts");
	pCommand->SetShortcut("Ctrl+T");
	AssertStringEqual(pCommand->GetShortcutString(), "Ctrl+T", "Command shortcut mismatch");
}

void DefaultTestCommandManager::TestCommandDescriptions()
{
	auto pCommand = IEngine::Get().GetCommandManager()->AddCommand("TestDescriptions");
	pCommand->SetDescription("Test command description");
	AssertStringEqual(pCommand->GetDescription(), "Test command description", "Command description mismatch");
}

void DefaultTestCommandManager::TestCommandManagerPersistence()
{
	auto pCommand = IEngine::Get().GetCommandManager()->AddCommand("TestPersistence");
	pCommand->SetDescription("Test persistence");
	pCommand->SetShortcut("Ctrl+P");

	IEngine::Get().GetCommandManager()->Save();
	IEngine::Get().GetCommandManager()->Load();

	auto loadedCmd =  IEngine::Get().GetCommandManager()->GetCommand("TestPersistence");
	AssertNotNull(loadedCmd.get(), "Command should be loaded");
	AssertStringEqual(loadedCmd->GetDescription(), "Test persistence", "Command description should persist");
	AssertStringEqual(loadedCmd->GetShortcutString(), "Ctrl+P", "Command shortcut should persist");
}

} // namespace RCLib::Tests::Impl