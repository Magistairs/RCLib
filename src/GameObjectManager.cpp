#include "GameObjectManager.h"
#include "Action.h"
#include "Engine.h"
#include "Logger.h"
#include "SmartPtr.h"
#include "MemoryTracking.h"
#include "GuidUtils.h"

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
	GameObjectPtr pNewObject = make_shared(GameObject);
	{
		std::lock_guard<std::shared_mutex> lock(m_objectsMutex);
		m_objects.push_back(pNewObject);
	}
	if (m_infoLogEnabled)
	{
		std::string message = "Object created. " + std::to_string(m_objects.size()) + " in total.";
		Logger::Get().Log(Logger::eInfo, message);
	}
	return pNewObject;
}

void GameObjectManager::RemoveObject(REFGUID objectGUID)
{
	std::lock_guard<std::shared_mutex> lock(m_objectsMutex);
	if (auto it = std::find_if(m_objects.begin(), m_objects.end(), GameObject::GUIDCompare(objectGUID)); it != m_objects.end())
	{
		GameObjectPtr                 pObject = (*it);
		pObject->Release();
		m_objects.erase(it);
		if (m_infoLogEnabled)
		{
			std::string message = "Object " + GuidUtils::GuidToString(objectGUID) + " removed.";
			Logger::Get().Log(Logger::eInfo, message);
		}
	}
	else
	{
		std::string message = "Object " + GuidUtils::GuidToString(objectGUID) + " asked to be removed but was not found!";
		Logger::Get().Log(Logger::eWarning, message);
	}
}

GameObjectPtr GameObjectManager::GetObject(REFGUID objectGUID) const
{
	std::shared_lock<std::shared_mutex> lock(m_objectsMutex);
	if (auto it = std::find_if(m_objects.begin(), m_objects.end(), GameObject::GUIDCompare(objectGUID)); it != m_objects.end())
	{
		return *it;
	}
	else
	{
		std::string message = "Object " + GuidUtils::GuidToString(objectGUID) + " asked but was not found!";
		Logger::Get().Log(Logger::eWarning, message);
	}
	return GameObjectPtr();
}

void RCLib::GameObjectManager::ForEachObject(const std::function<void(GameObjectPtr)>& func) const
{
	std::shared_lock<std::shared_mutex> lock(m_objectsMutex);
	for (GameObjectPtr pObject : m_objects)
	{
		func(pObject);
	}
}
