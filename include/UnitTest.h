#pragma once
#include "EventSystem.h"
#include "StdIncludes.h"
#include "TaskManager.h"

namespace RCLib
{

class RCLIB_API UnitTest
{
public:
	UnitTest()  = default;
	~UnitTest() = default;

	void Run();

	EventSystem& GetEventSystem() { return m_eventSystem; }
	TaskManager& GetTaskManager() { return m_taskManager; }

private:
	EventSystem m_eventSystem;
	TaskManager m_taskManager;
};
} // namespace RCLib