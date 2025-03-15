#include "UnitTest.h"
#include "Engine.h"
#include "GameObjectManager.h"
#include "Logger.h"

using namespace RCLib;

namespace RCLib
{
class ObjectsManagerTests
{
public:
	ObjectsManagerTests(/*sf::Int32 timeOut*/ UnitTest* pUnitTest)
	  : m_pUnitTest(pUnitTest) // timeOut(timeOut)
	{
	}

	void Run();

	GameObjectPtr GetRandomObject(bool remove)
	{
		std::lock_guard<std::shared_mutex> lock(m_objectsMutex);
		if (!m_objects.empty())
		{
			int           objectToRemove = std::rand() % m_objects.size();
			GameObjectPtr pObject        = m_objects[objectToRemove];
			if (remove)
			{
				m_objects.erase(m_objects.begin() + objectToRemove);
				++m_objectsRemoved;
			}
			return pObject;
		}
		return GameObjectPtr();
	}

	void Stop()
	{
		while (m_runningTasks > 0 /*&& m_clock.getElapsedTime().asMilliseconds() < timeOut*/) {}
		assert(m_runningTasks == 0 && m_objects.empty());
		m_stop = true; // avoid infinite loops but if they need to be stopped there is a problem
	}

private:
	static constexpr int s_numOfWorkingThreadsByAction = 100;
	static constexpr int s_numObjectsToAddByThread     = 100;
	static constexpr int s_numObjectsToAdd             = s_numOfWorkingThreadsByAction * s_numObjectsToAddByThread;

	std::atomic<int> m_runningTasks{0};
	std::atomic<int> m_objectsAdded{0};
	std::atomic<int> m_objectsRemoved{0};

	std::shared_mutex          m_objectsMutex;
	std::vector<GameObjectPtr> m_objects;
	std::shared_mutex          m_componentsMutex;

	// sf::Int32 timeOut;
	// sf::Clock m_clock;
	bool m_stop = false;

	UnitTest* m_pUnitTest = nullptr;
};
} // namespace RCLib
void RCLib::ObjectsManagerTests::Run()
{
	Logger::Get().Log(Logger::eInfo, "Begin ObjectsManager test");

	// ActionManager::Get().ExecuteAction("GameObjectManager_enable_log");

	// Create objects
	++m_runningTasks;
	m_pUnitTest->GetTaskManager().AddTask([this]() {
		for (int i = 0; i < s_numOfWorkingThreadsByAction; i++)
		{
			++m_runningTasks;
			m_pUnitTest->GetTaskManager().AddTask([this]() {
				for (int j = 0; j < s_numObjectsToAddByThread; j++)
				{
					GameObjectPtr pObject = GameObjectManager::Get().CreateObject();

					std::lock_guard<std::shared_mutex> lock(m_objectsMutex);
					m_objects.push_back(pObject);
					++m_objectsAdded;
				}
				--m_runningTasks;
			});
		}
		--m_runningTasks;
	});

	// Destroy objects
	++m_runningTasks;
	m_pUnitTest->GetTaskManager().AddTask([this]() {
		for (int i = 0; i < s_numOfWorkingThreadsByAction; i++)
		{
			++m_runningTasks;
			m_pUnitTest->GetTaskManager().AddTask([this]() {
				while ((m_objectsAdded < s_numObjectsToAdd || m_objectsAdded > m_objectsRemoved) && !m_stop)
				{
					if (GameObjectPtr pObject = GetRandomObject(true))
					{
						GameObjectManager::Get().RemoveObject(pObject->GetGUID());
					}
				}
				--m_runningTasks;
			});
		}
		--m_runningTasks;
	});

	Stop();
	Logger::Get().Log(Logger::eInfo, "End ObjectsManager test");
}

void RCLib::UnitTest::Run()
{
	ObjectsManagerTests(/*9999*/ this).Run();

	// EventSystem
	if (false)
	{
		std::srand((unsigned int)std::time(NULL));

		for (int i = 0; i < 100; i++)
		{
			m_taskManager.AddTask([this]() {
				for (int j = 0; j < 100; j++)
				{
					int a = std::rand() % 4;
					switch (a)
					{
					case 0:
						{
							m_eventSystem.AddQueuedEventCallback(this, std::rand() % 4, []() {});
						}
						break;
					case 1:
						{
							m_eventSystem.RemoveQueuedEventCallback(this, std::rand() % 4);
						}
						break;
					case 2:
						{
							m_eventSystem.QueueEvent(std::rand() % 4);
						}
						break;
					default:
						{
							m_eventSystem.ProcessEvents();
						}
						break;
					}
				}
			});
		}
	}
}
