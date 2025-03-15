#include "Action.h"

#include "Engine.h"
#include "Logger.h"

#include <algorithm>
#include <cctype>

using namespace RCLib;

Action::Action(const char* name)
	: m_name(name)
{
	//SfmlUtils::ClearKeyEvent(m_shortcut);
	Engine::Get().GetEventSystem().AddQueuedEventCallback(this, Engine::eProcessedEvents, [this] { m_eventSystem.ProcessEvents(); });
	m_eventSystem.AddQueuedEventCallback(this, Engine::eActionExecuteRequested, [this] { Execute(); });
}

void Action::Execute()
{
	if (m_callback)
	{
		if (s_logEnabled)
			Logger::Get().Log(Logger::eInfo, "Action will be executed: " + m_name);

		m_callback();
	}
	else if (s_logEnabled)
	{
		Logger::Get().Log(Logger::eError, "Action has no callback: " + m_name);
	}
}

void RCLib::Action::ExecuteLater()
{
	m_eventSystem.QueueEvent(Engine::eActionExecuteRequested);
}

void Action::SetCallback(std::function<void()>&& callback)
{
	m_callback = std::move(callback);

	for (auto it = s_actionsToExecute.begin(); it != s_actionsToExecute.end(); ++it)
	{
		if (m_name == (*it))
		{
			Execute();
			s_actionsToExecute.erase(it);
			return;
		}
	}
}

void Action::SetShortcut(const std::string& shortcut)
{
	m_shortcutStr = shortcut;
	//	SfmlUtils::ClearKeyEvent(m_shortcut);
	//	SfmlUtils::StringToKeyEvent(shortcut, m_shortcut);
}

std::string Action::GetShortcutString() const
{
	return m_shortcutStr;
}

void Action::Serialize(bool load, ObjectPtr pJsonObject)
{
}

void Action::Load()
{

}

void RCLib::Action::SetActionsToExecute(std::vector<std::string>& actionsToExecute)
{
	s_actionsToExecute.swap(actionsToExecute);
}