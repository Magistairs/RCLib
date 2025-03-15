#pragma once
#include "ActionManager.h"
#include "DLLExport.h"
#include "EventSystem.h"
#include "Logger.h"
#include "GameObjectManager.h"
#include "TaskManager.h"
#include "UnitTest.h"

#include <memory>

namespace RCLib
{
	class RCLIB_API Engine
	{
	public:
		enum EEvent
		{
			eObjectCreated = 0,
			eObjectLoaded,
			eProcessedEvents,
			eActionExecuteRequested,
			eEventCount
		};

		static Engine& Get();

		Engine();
		~Engine();

		void Init();
		void ProcessEvents();
		void Update();
		void Release();

		EventSystem& GetEventSystem() { return *m_pEventSystem; }
		TaskManager& GetTaskManager() { return *m_pTaskManager; }

	private:
		std::unique_ptr<ActionManager>     m_pActionManager;
		std::unique_ptr<EventSystem>       m_pEventSystem;
		std::unique_ptr<GameObjectManager> m_pGameObjectManager;
		std::unique_ptr<Logger>            m_pLogger;
		std::unique_ptr<TaskManager>       m_pTaskManager;
		std::unique_ptr<UnitTest>          m_pUnitTest;
	};
} // namespace RCLib