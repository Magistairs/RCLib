#include "ActionManager.h"

#include "Logger.h"
#include "MemoryTracking.h"

#include <assert.h>
#include <utility>

using namespace RCLib;

ActionManager* s_pActionsManager = nullptr;

ActionManager& ActionManager::Get()
{
	assert(s_pActionsManager);
	return *s_pActionsManager;
}

ActionManager::ActionManager()
{
	assert(!s_pActionsManager);
	s_pActionsManager = this;
}

ActionManager::~ActionManager()
{
	s_pActionsManager = nullptr;
}

ActionPtr ActionManager::AddAction(const char* name)
{
	ActionPtr pAction = make_shared(Action, name);
	m_actions.push_back(pAction);

	if (IsLoaded())
	{
		// If the manager is already loaded and a new action is added, load it alone
		// But it should be avoided in general
		std::string message = "Action added after loading: " + pAction->GetName();
		Logger::Get().Log(Logger::eWarning, message);
		pAction->StartLoading();
	}

	return pAction;
}

bool RCLib::ActionManager::ExecuteAction(const char* name)
{
	for (ActionPtr& action : m_actions)
	{
		if (action->GetName() == name)
		{
			action->Execute();
			return true;
		}
	}
	return false;
}

void ActionManager::Serialize(bool load, ObjectPtr pJsonObject)
{
	for (ActionPtr& action : m_actions)
	{
		action->Serialize(load, pJsonObject);
	}
}

void ActionManager::Save()
{
	//Serializer    serializer;
	//ObjectPtr pRoot = serializer.CreateRoot();
	//Serialize(false, pRoot);
	//serializer.Write("action.json");
}

void ActionManager::Load()
{
	//Serializer serializer;
	//if (serializer.Parse("action.json"))
	//{
	//	ObjectPtr pRoot = serializer.GetRoot();
	//	Serialize(true, pRoot);
	//}
}
