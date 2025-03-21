#include "DefaultEngine.h"
#include "RCLib.h"

// Static member definition
RCLib::IEngine& RCLib::IEngine::Get()
{
	static RCLib::Impl::DefaultEngine instance;
	return instance;
}

namespace RCLib::Impl
{

DefaultEngine::DefaultEngine()
{
	// Create core managers using factory
	m_logger         = IFactory::Get()->Create<ILogger>();
	m_memoryManager  = IFactory::Get()->Create<IMemoryManager>();
	m_commandManager = IFactory::Get()->Create<ICommandManager>();
	m_entityManager  = IFactory::Get()->Create<IEntityManager>();
	m_eventManager   = IFactory::Get()->Create<IEventManager>();
	m_taskManager    = IFactory::Get()->Create<ITaskManager>();
	m_messageManager = IFactory::Get()->Create<IMessageManager>();

	// Add managers to the list in initialization order
	m_managers.push_back(MakeShared<DefaultStateWrapper<ILogger>>("Logger", m_logger));
	m_managers.push_back(MakeShared<DefaultStateWrapper<IMemoryManager>>("MemoryManager", m_memoryManager));
	m_managers.push_back(MakeShared<DefaultStateWrapper<ICommandManager>>("CommandManager", m_commandManager));
	m_managers.push_back(MakeShared<DefaultStateWrapper<IEntityManager>>("EntityManager", m_entityManager));
	m_managers.push_back(MakeShared<DefaultStateWrapper<IEventManager>>("EventManager", m_eventManager));
	m_managers.push_back(MakeShared<DefaultStateWrapper<ITaskManager>>("TaskManager", m_taskManager));
	m_managers.push_back(MakeShared<DefaultStateWrapper<IMessageManager>>("MessageManager", m_messageManager));
}

DefaultEngine::~DefaultEngine() {}

bool DefaultEngine::OnInitialize()
{
	// Initialize managers in order
	for (auto& manager : m_managers)
	{
		if (manager && !manager->IsInitialized())
		{
			return false;
		}
	}

	m_logger->Info("All managers initialized successfully");
	return true;
}

bool DefaultEngine::OnUpdate()
{
	// Update managers in order
	for (auto& manager : m_managers)
	{
		if (manager && !manager->IsInitialized())
		{
			return false;
		}
		manager->Update();
	}

	return true;
}

void DefaultEngine::OnRelease()
{
	// Release managers in reverse order
	for (auto it = m_managers.rbegin(); it != m_managers.rend(); ++it)
	{
		if (*it)
		{
			(*it)->Release();
		}
	}

	if (m_logger)
	{
		m_logger->Info("All managers released successfully");
	}

	m_managers.clear();
}

void DefaultEngine::RegisterCustomManager(IStateWrapperPtr manager)
{
	if (manager)
	{
		m_managers.push_back(manager);
		m_logger->Info("Custom manager registered");
	}
}

const ITaskManagerPtr DefaultEngine::GetTaskManager()
{
	return m_taskManager;
}

const IEntityManagerPtr DefaultEngine::GetEntityManager()
{
	return m_entityManager;
}

const ILoggerPtr DefaultEngine::GetLogger()
{
	return m_logger;
}

const ICommandManagerPtr DefaultEngine::GetCommandManager()
{
	return m_commandManager;
}

const IEventManagerPtr DefaultEngine::GetEventManager()
{
	return m_eventManager;
}

const IMessageManagerPtr DefaultEngine::GetMessageManager()
{
	return m_messageManager;
}

const IMemoryManagerPtr DefaultEngine::GetMemoryManager()
{
	return m_memoryManager;
}

} // namespace RCLib::Impl