#include "Engine.h"
#include "GameObject.h"
#include "Profiler.h"

#include <assert.h>

using namespace RCLib;

Engine* s_pEngine = nullptr;

Engine& Engine::Get()
{
	assert(s_pEngine);
	return *s_pEngine;
}

Engine::Engine()
{
	assert(!s_pEngine);
	s_pEngine = this;
	m_pActionManager = std::make_unique<ActionManager>();
	m_pEventSystem = std::make_unique<EventSystem>();
	m_pGameObjectManager = std::make_unique<GameObjectManager>();
	m_pLogger = std::make_unique<Logger>();
	m_pTaskManager = std::make_unique<TaskManager>();
	m_pUnitTest = std::make_unique<UnitTest>();
}

Engine::~Engine()
{
	s_pEngine = nullptr;
}

void Engine::Init()
{
	m_pGameObjectManager->Init();

	ActionPtr pAction = ActionManager::Get().AddAction("EventSystem_enable_log");
	pAction->SetCallback([] { EventSystem::EnableLog(true); });

	pAction = ActionManager::Get().AddAction("EventSystem_disable_log");
	pAction->SetCallback([] { EventSystem::EnableLog(false); });

	pAction = ActionManager::Get().AddAction("run_tests");
	pAction->SetCallback([this] { m_pUnitTest->Run(); });
}

void RCLib::Engine::ProcessEvents()
{
	m_pEventSystem->QueueEvent(eProcessedEvents);
	m_pEventSystem->ProcessEvents();
}

void Engine::Update()
{
	// PROFILE_FUNCTION
	m_pGameObjectManager->ForEachObject([](GameObjectPtr pObject) { pObject->Update(); });
}

void Engine::Release()
{
	m_pActionManager->StartSaving();
}
