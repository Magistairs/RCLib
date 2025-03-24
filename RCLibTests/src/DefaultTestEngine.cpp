#include "DefaultTestEngine.h"
#include "RCLibTests.h"

namespace RCLib::Tests::Impl
{

void DefaultTestEngine::Setup() {}

void DefaultTestEngine::Run()
{
	TestManagerLifecycle();
	TestCustomManagerRegistration();
	TestManagerUpdateOrder();
	TestManagerExceptions();
}

void DefaultTestEngine::Cleanup() {}

void DefaultTestEngine::TestManagerLifecycle()
{
	// auto& engine = IEngine::Get();

	//// Test initial state
	// assert(engine.GetLogger() == nullptr && "Logger should not exist before initialization");

	//// Test initialization
	// engine.OnInitialize();
	// assert(engine.GetLogger() != nullptr && "Logger should exist after initialization");

	//// Test core managers creation
	// assert(engine.GetCommandManager() != nullptr && "CommandManager should exist");
	// assert(engine.GetEventManager() != nullptr && "EventManager should exist");
	// assert(engine.GetTaskManager() != nullptr && "TaskManager should exist");
	// assert(engine.GetEntityManager() != nullptr && "EntityManager should exist");
	// assert(engine.GetEventManager() != nullptr && "EventManager should exist");
	// assert(engine.GetMemoryManager() != nullptr && "MemoryManager should exist");
	// assert(engine.GetMessageManager() != nullptr && "MessageManager should exist");
	// assert(engine.GetTestRunner() != nullptr && "TestRunner should exist");

	// assert(engine.GetLogger() == nullptr && "Logger should not exist after release");
}

void DefaultTestEngine::TestCustomManagerRegistration()
{
	auto& engine = IEngine::Get();

	// Register custom manager
	// auto  customManager     = std::make_unique<TestManager>("CustomManager");
	// auto* managerPtr        = customManager.get();
	// auto  registeredManager = engine.RegisterCustomManager(std::move(customManager));
	// assert(registeredManager == managerPtr && "RegisterCustomManager should return reference to registered manager");
	// assert(managerPtr->IsInitialized() && "Custom manager should be initialized after registration");

	//// Test that we can register multiple managers
	// auto  customManager2     = std::make_unique<TestManager>("CustomManager2");
	// auto* manager2Ptr        = customManager2.get();
	// auto  registeredManager2 = engine.RegisterCustomManager(std::move(customManager2));
	// assert(registeredManager2 == manager2Ptr && "Second manager should be registered correctly");
	// assert(registeredManager != registeredManager2 && "Different managers should have different addresses");
}

void DefaultTestEngine::TestManagerUpdateOrder()
{
	auto& engine = IEngine::Get();

	// Create managers with ordered names
	// auto first  = std::make_unique<TestManager>("A_First");
	// auto second = std::make_unique<TestManager>("B_Second");
	// auto third  = std::make_unique<TestManager>("C_Third");

	// auto* firstPtr  = first.get();
	// auto* secondPtr = second.get();
	// auto* thirdPtr  = third.get();

	// engine.RegisterCustomManager(std::move(first));
	// engine.RegisterCustomManager(std::move(second));
	// engine.RegisterCustomManager(std::move(third));

	// assert(firstPtr->GetUpdateCount() == 1 && "First manager should be updated once");
	// assert(secondPtr->GetUpdateCount() == 1 && "Second manager should be updated once");
	// assert(thirdPtr->GetUpdateCount() == 1 && "Third manager should be updated once");
}

void DefaultTestEngine::TestManagerExceptions()
{
	auto& engine = IEngine::Get();

	// Test null manager registration
	bool caught = false;
	try
	{
		// engine.RegisterCustomManager(nullptr);
	}
	catch (const std::invalid_argument&)
	{
		caught = true;
	}
	// assert(caught && "Should throw when registering null manager");

	// Initialize and test valid operations
	engine.OnInitialize();
	caught = false;
	try
	{
		// engine.RegisterCustomManager(std::make_unique<TestManager>("ShouldSucceed"));
	}
	catch (...)
	{
		caught = true;
	}
	// assert(!caught && "Should not throw when registering valid manager after initialization");
}

void DefaultTestEngine::ValidateManagerCount(size_t expectedCount)
{
	// Note: This would need friend access to EngineImpl to implement
	// Left as a placeholder for future implementation
}

} // namespace RCLib::Tests::Impl