#pragma once
#include "IEngine.h"
#include "RCLib_Fwd.h"

#include <vector>

namespace RCLib::Impl
{
/**
 * @brief Default implementation of the engine system
 * @ingroup Core
 *
 * @details Main engine implementation that manages:
 * - System lifecycle (initialization, update, shutdown)
 * - Manager creation and registration
 * - Resource management
 *
 * Uses a factory system for creating managers and components.
 *
 * @note Thread-safe singleton implementation
 * @see IFactory
 * @see IManager
 */
class RCLIB_API DefaultEngine : public IEngine
{
public:
	friend IEngine& IEngine::Get();

	DefaultEngine();
	~DefaultEngine() override;

	// IEngine interface
	bool OnInitialize() override;
	bool OnUpdate() override;
	void OnRelease() override;

	// Custom manager registration
	void RegisterCustomManager(IStateWrapperPtr manager) override;

	// Manager getters
	const ITaskManagerPtr    GetTaskManager() override;
	const IEntityManagerPtr  GetEntityManager() override;
	const ILoggerPtr         GetLogger() override;
	const ICommandManagerPtr GetCommandManager() override;
	const IEventManagerPtr   GetEventManager() override;
	const IMessageManagerPtr GetMessageManager() override;
	const IMemoryManagerPtr  GetMemoryManager() override;

protected:
	void InitializeManagers();
	void CreateCoreManagers();

private:
	// Core managers
	ILoggerPtr         m_logger;
	IMemoryManagerPtr  m_memoryManager;
	ICommandManagerPtr m_commandManager;
	IEntityManagerPtr  m_entityManager;
	IEventManagerPtr   m_eventManager;
	ITaskManagerPtr    m_taskManager;
	IMessageManagerPtr m_messageManager;

	// All managers (core and custom) stored in initialization order
	std::vector<IStateWrapperPtr> m_managers;
};

} // namespace RCLib::Impl