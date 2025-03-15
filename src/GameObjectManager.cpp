#include "GameObjectManager.h"
#include "Action.h"
#include "Engine.h"
#include "Logger.h"
#include "MemoryTracking.h"
#include "SmartPtr.h"

#include <assert.h>
#include <string>

using namespace RCLib;

GameObjectManager* s_pGameObjectManager = nullptr;

GameObjectManager& GameObjectManager::Get()
{
	assert(s_pGameObjectManager);
	return *s_pGameObjectManager;
}

GameObjectManager::GameObjectManager()
{
	assert(!s_pGameObjectManager);
	s_pGameObjectManager = this;
}

GameObjectManager::~GameObjectManager()
{
	s_pGameObjectManager = nullptr;
}

void RCLib::GameObjectManager::Init()
{
	ActionPtr pAction = ActionManager::Get().AddAction("GameObjectManager_enable_log");
	pAction->SetCallback([this] { m_infoLogEnabled = true; });

	pAction = ActionManager::Get().AddAction("GameObjectManager_disable_log");
	pAction->SetCallback([this] { m_infoLogEnabled = false; });

	pAction = ActionManager::Get().AddAction("create_object");
	pAction->SetCallback([this] { CreateObject(); });
}

GameObjectPtr GameObjectManager::CreateObject()
{
	std::lock_guard<std::shared_mutex> lock(m_objectsMutex);

	GameObjectPtr pObject = std::make_shared<GameObject>();
	m_objects.push_back(pObject);

	if (m_infoLogEnabled)
	{
		std::string message = "Object " + std::to_string(pObject->GetGUID()) + " created.";
		Logger::Get().Log(Logger::eInfo, message);
	}

	return pObject;
}

void GameObjectManager::RemoveObject(uint64_t objectGuid)
{
	std::lock_guard<std::shared_mutex> lock(m_objectsMutex);

	if (auto it = std::find_if(m_objects.begin(), m_objects.end(), GameObject::GUIDCompare(objectGuid)); it != m_objects.end())
	{
		m_objects.erase(it);

		if (m_infoLogEnabled)
		{
			std::string message = "Object " + std::to_string(objectGuid) + " removed.";
			Logger::Get().Log(Logger::eInfo, message);
		}
	}
	else
	{
		std::string message = "Object " + std::to_string(objectGuid) + " asked to be removed but was not found!";
		Logger::Get().Log(Logger::eWarning, message);
	}
}

GameObjectPtr GameObjectManager::GetObject(uint64_t objectGuid) const
{
	std::shared_lock<std::shared_mutex> lock(m_objectsMutex);

	if (auto it = std::find_if(m_objects.begin(), m_objects.end(), GameObject::GUIDCompare(objectGuid)); it != m_objects.end())
	{
		return *it;
	}

	std::string message = "Object " + std::to_string(objectGuid) + " asked but was not found!";
	Logger::Get().Log(Logger::eWarning, message);

	return nullptr;
}

void RCLib::GameObjectManager::ForEachObject(const std::function<void(GameObjectPtr)>& func) const
{
	std::shared_lock<std::shared_mutex> lock(m_objectsMutex);

	for (const auto& pObject : m_objects)
	{
		func(pObject);
	}
}
