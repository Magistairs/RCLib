#include "DefaultTestEntityManager.h"
#include "DefaultTest.h"
#include "RCLibTests.h"
#include "IEngine.h"
#include <random>
#include <thread>

namespace RCLib::Tests::Impl
{
DefaultTestEntityManager::DefaultTestEntityManager()
	: DefaultTest("TestEntityManager")
{
}

DefaultTestEntityManager::~DefaultTestEntityManager()
{
	Cleanup();
}

void DefaultTestEntityManager::Setup()
{
	m_runningTasks    = 0;
	m_entitiesAdded   = 0;
	m_entitiesRemoved = 0;
	m_stop            = false;
	m_entities.clear();
}

void DefaultTestEntityManager::Run()
{
	RunTest("TestEntityManager", [this]() {
		CreateEntities();
		DestroyEntities();
		Stop();

		AssertEqual(m_runningTasks.load(), 0, "All tasks should be completed");
		AssertEqual(m_entitiesAdded.load(), s_numEntitiesToAdd, "All entities should have been added");
		AssertEqual(m_entitiesRemoved.load(), s_numEntitiesToAdd, "All entities should have been removed");
		AssertTrue(m_entities.empty(), "Entities vector should be empty");
	});
}

void DefaultTestEntityManager::Cleanup()
{
	m_stop = true;
	Stop();
	m_entities.clear();
}

std::string_view DefaultTestEntityManager::GetName() const
{
	return "TestEntityManager";
}

std::string_view DefaultTestEntityManager::GetDescription() const
{
	return "Tests the EntityManager with concurrent creation and destruction of entities";
}

RCLib::IEntityPtr DefaultTestEntityManager::GetRandomEntity(bool remove)
{
	std::shared_lock<std::shared_mutex> lock(m_entitiesMutex);
	if (m_entities.empty())
	{
		return nullptr;
	}

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, static_cast<int>(m_entities.size()) - 1);
	int index = dis(gen);

	RCLib::IEntityPtr entity = m_entities[index];
	if (remove)
	{
		std::unique_lock<std::shared_mutex> uniqueLock(m_entitiesMutex);
		m_entities.erase(m_entities.begin() + index);
	}
	return entity;
}

void DefaultTestEntityManager::Stop()
{
	while (m_runningTasks > 0)
	{
		auto taskManager = IEngine::Get().GetTaskManager();
		while (taskManager->HasTasks())
		{
			std::this_thread::yield();
		}
	}
	AssertTrue(m_entities.empty(), "Entities vector should be empty after stopping");
}

void DefaultTestEntityManager::CreateEntities()
{
	for (int i = 0; i < s_numOfWorkingThreadsByAction; ++i)
	{
		++m_runningTasks;
		IEngine::Get().GetTaskManager()->AddTask([this]() {
			for (int j = 0; j < s_numEntitiesToAddByThread && !m_stop; ++j)
			{
				auto entityId = IEngine::Get().GetEntityManager()->CreateEntity();
				auto entity   = IEngine::Get().GetEntityManager()->GetEntity(entityId);
				{
					std::unique_lock<std::shared_mutex> lock(m_entitiesMutex);
					m_entities.push_back(entity);
				}
				++m_entitiesAdded;
			}
			--m_runningTasks;
		});
	}
	auto taskManager = IEngine::Get().GetTaskManager();
	while (taskManager->HasTasks())
	{
		std::this_thread::yield();
	}
}

void DefaultTestEntityManager::DestroyEntities()
{
	for (int i = 0; i < s_numOfWorkingThreadsByAction; ++i)
	{
		++m_runningTasks;
		IEngine::Get().GetTaskManager()->AddTask([this]() {
			while (!m_stop)
			{
				auto entity = GetRandomEntity(true);
				if (!entity)
				{
					break;
				}
				IEngine::Get().GetEntityManager()->DestroyEntity(entity);
				++m_entitiesRemoved;
			}
			--m_runningTasks;
		});
	}
}
} // namespace RCLib::Tests::Impl