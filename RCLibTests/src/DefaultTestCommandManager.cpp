#include "DefaultTestCommandManager.h"
#include "RCLibTests.h"

namespace RCLib::Impl
{
DefaultTestCommandManager::DefaultTestCommandManager()
  : DefaultTest("CommandManager")
{
}

void DefaultTestCommandManager::Setup()
{
	auto& engine = IEngine::Get();
	m_manager = engine.GetFactory()->Create<DefaultCommandManager>();
	m_manager->OnInitialize();
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
	if (m_manager)
	{
		m_manager->OnRelease();
		m_manager.reset();
	}
}

void DefaultTestCommandManager::TestCommandCreation()
{
	auto pCommand = m_manager->AddCommand("TestCommand");
	AssertNotNull(pCommand.get(), "Command should be created");
	AssertStringEqual(pCommand->GetName(), "TestCommand", "Command name should match");
}

void DefaultTestCommandManager::TestCommandExecution()
{
	bool executed = false;
	auto pCommand = m_manager->AddCommand("TestExecution");
	pCommand->SetCallback([&executed]() { executed = true; });
	pCommand->Execute();
	AssertTrue(executed, "Command execution failed");
}

void DefaultTestCommandManager::TestCommandQueue()
{
	size_t initialQueueSize = m_manager->GetQueueSize();
	auto pCommand = m_manager->AddCommand("TestQueue");
	pCommand->ExecuteLater();
	AssertEqual(m_manager->GetQueueSize(), initialQueueSize + 1, "Command queue size mismatch");
}

void DefaultTestCommandManager::TestCommandUndo()
{
	bool executed = false;
	bool undone = false;
	auto pCommand = m_manager->AddCommand("TestUndo");
	pCommand->SetCallback([&executed]() { executed = true; });
	auto defaultCommand = std::dynamic_pointer_cast<DefaultCommand>(pCommand);
	defaultCommand->SetUndoCallback([&undone]() { undone = true; });
	pCommand->Execute();
	AssertTrue(executed, "Command execution failed");
	m_manager->Undo();
	AssertTrue(undone, "Command undo failed");
}

void DefaultTestCommandManager::TestCommandExecuteLater()
{
	bool executed = false;
	auto pCommand = m_manager->AddCommand("TestExecuteLater");
	pCommand->SetCallback([&executed]() { executed = true; });
	pCommand->ExecuteLater();
	AssertFalse(executed, "Command executed immediately instead of being queued");
	m_manager->OnUpdate();
	AssertTrue(executed, "Command not executed after update");
}

void DefaultTestCommandManager::TestCommandShortcuts()
{
	auto pCommand = m_manager->AddCommand("TestShortcuts");
	pCommand->SetShortcut("Ctrl+T");
	AssertStringEqual(pCommand->GetShortcutString(), "Ctrl+T", "Command shortcut mismatch");
}

void DefaultTestCommandManager::TestCommandDescriptions()
{
	auto pCommand = m_manager->AddCommand("TestDescriptions");
	pCommand->SetDescription("Test command description");
	AssertStringEqual(pCommand->GetDescription(), "Test command description", "Command description mismatch");
}

void DefaultTestCommandManager::TestCommandManagerPersistence()
{
	auto pCommand = m_manager->AddCommand("TestPersistence");
	pCommand->SetDescription("Test persistence");
	pCommand->SetShortcut("Ctrl+P");

	m_manager->Save();
	m_manager->Load();

	auto defaultManager = std::dynamic_pointer_cast<DefaultCommandManager>(m_manager);
	AssertNotNull(defaultManager.get(), "Manager should be DefaultCommandManager");
	
	auto loadedCmd = defaultManager->GetCommand("TestPersistence");
	AssertNotNull(loadedCmd.get(), "Command should be loaded");
	AssertStringEqual(loadedCmd->GetDescription(), "Test persistence", "Command description should persist");
	AssertStringEqual(loadedCmd->GetShortcutString(), "Ctrl+P", "Command shortcut should persist");
}

} // namespace RCLib::Impl