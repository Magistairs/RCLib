#include "DefaultTestEngine.h"
#include "RCLibTests.h"

namespace RCLib::Impl
{

class TestManager : public ITriStateAccessor
{
public:
	TestManager(std::string_view name)
	  : m_name(name)
	{
	}

	void Initialize() override { SetInitialized(true); }
	void Update() override { m_updateCount++; }
	void Release() override { SetInitialized(false); }

	bool             IsInitialized() const override { return m_isInitialized; }
	int              GetUpdateCount() const { return m_updateCount; }
	std::string_view GetName() const override { return m_name; }
	void             SetInitialized(bool initialized) override { m_isInitialized = initialized; }

private:
	std::string m_name;
	bool        m_isInitialized{false};
	int         m_updateCount{0};
};

void TestEngineImpl::Setup()
{
	// Make sure engine is in a clean state
	auto& engine = IEngine::Get();
	if (engine.GetLogger() != nullptr)
	{
		engine.Release();
	}
}

void TestEngineImpl::Run()
{
	TestManagerLifecycle();
	TestCustomManagerRegistration();
	TestManagerUpdateOrder();
	TestManagerExceptions();
}

void TestEngineImpl::Cleanup()
{
	auto& engine = IEngine::Get();
	engine.Release();
}

void TestEngineImpl::TestManagerLifecycle()
{
	auto& engine = IEngine::Get();

	// Test initial state
	assert(engine.GetLogger() == nullptr && "Logger should not exist before initialization");

	// Test initialization
	engine.OnInitialize();
	assert(engine.GetLogger() != nullptr && "Logger should exist after initialization");

	// Test core managers creation
	assert(engine.GetCommandManager() != nullptr && "CommandManager should exist");
	assert(engine.GetEventManager() != nullptr && "EventManager should exist");
	assert(engine.GetTaskManager() != nullptr && "TaskManager should exist");
	assert(engine.GetEntityManager() != nullptr && "EntityManager should exist");
	assert(engine.GetEventManager() != nullptr && "EventManager should exist");
	assert(engine.GetMemoryManager() != nullptr && "MemoryManager should exist");
	assert(engine.GetMessageManager() != nullptr && "MessageManager should exist");
	assert(engine.GetTestRunner() != nullptr && "TestRunner should exist");

	// Test update
	engine.Update();

	// Test release
	engine.Release();
	assert(engine.GetLogger() == nullptr && "Logger should not exist after release");
}

void TestEngineImpl::TestCustomManagerRegistration()
{
	auto& engine = IEngine::Get();
	engine.OnInitialize();

	// Register custom manager
	auto  customManager     = std::make_unique<TestManager>("CustomManager");
	auto* managerPtr        = customManager.get();
	auto  registeredManager = engine.RegisterCustomManager(std::move(customManager));
	assert(registeredManager == managerPtr && "RegisterCustomManager should return reference to registered manager");
	assert(managerPtr->IsInitialized() && "Custom manager should be initialized after registration");

	// Test that we can register multiple managers
	auto  customManager2     = std::make_unique<TestManager>("CustomManager2");
	auto* manager2Ptr        = customManager2.get();
	auto  registeredManager2 = engine.RegisterCustomManager(std::move(customManager2));
	assert(registeredManager2 == manager2Ptr && "Second manager should be registered correctly");
	assert(registeredManager != registeredManager2 && "Different managers should have different addresses");

	engine.Release();
}

void TestEngineImpl::TestManagerUpdateOrder()
{
	auto& engine = IEngine::Get();
	engine.OnInitialize();

	// Create managers with ordered names
	auto first  = std::make_unique<TestManager>("A_First");
	auto second = std::make_unique<TestManager>("B_Second");
	auto third  = std::make_unique<TestManager>("C_Third");

	auto* firstPtr  = first.get();
	auto* secondPtr = second.get();
	auto* thirdPtr  = third.get();

	engine.RegisterCustomManager(std::move(first));
	engine.RegisterCustomManager(std::move(second));
	engine.RegisterCustomManager(std::move(third));

	// Test update propagation
	engine.Update();

	assert(firstPtr->GetUpdateCount() == 1 && "First manager should be updated once");
	assert(secondPtr->GetUpdateCount() == 1 && "Second manager should be updated once");
	assert(thirdPtr->GetUpdateCount() == 1 && "Third manager should be updated once");

	// Validate initialization state
	ValidateManagerState(*firstPtr, true);
	ValidateManagerState(*secondPtr, true);
	ValidateManagerState(*thirdPtr, true);

	engine.Release();

	// Validate release state
	ValidateManagerState(*firstPtr, false);
	ValidateManagerState(*secondPtr, false);
	ValidateManagerState(*thirdPtr, false);
}

void TestEngineImpl::TestManagerExceptions()
{
	auto& engine = IEngine::Get();

	// Test null manager registration
	bool caught = false;
	try
	{
		engine.RegisterCustomManager(nullptr);
	}
	catch (const std::invalid_argument&)
	{
		caught = true;
	}
	assert(caught && "Should throw when registering null manager");

	// Initialize and test valid operations
	engine.OnInitialize();
	caught = false;
	try
	{
		engine.RegisterCustomManager(std::make_unique<TestManager>("ShouldSucceed"));
	}
	catch (...)
	{
		caught = true;
	}
	assert(!caught && "Should not throw when registering valid manager after initialization");

	engine.Release();
}

void TestEngineImpl::ValidateManagerState(const ITriStateAccessor& manager, bool expectedState)
{
	auto* testManager = dynamic_cast<const TestManager*>(&manager);
	assert(testManager && "Manager should be a TestManager");
	assert(testManager->IsInitialized() == expectedState && "Manager initialization state does not match expected state");
}

void TestEngineImpl::ValidateManagerCount(size_t expectedCount)
{
	// Note: This would need friend access to EngineImpl to implement
	// Left as a placeholder for future implementation
}

} // namespace RCLib::Impl