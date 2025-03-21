#include "DefaultCommand.h"
#include "RCLib.h"

namespace RCLib::Impl
{

DefaultCommand::DefaultCommand(std::string_view name)
  : m_name(name)
{
}

DefaultCommand::~DefaultCommand() = default;

void DefaultCommand::Execute()
{
	if (m_callback)
	{
		IEngine::Get().GetLogger()->Info(std::string("Command will be executed: ").append(m_name));
		m_callback();
	}
	else
	{
		IEngine::Get().GetLogger()->Error(std::string("Command has no callback: ").append(m_name));
	}
}

void DefaultCommand::Undo()
{
	if (m_undoCallback)
	{
		IEngine::Get().GetLogger()->Info(std::string("Command will be undone: ").append(m_name));
		m_undoCallback();
	}
	else
	{
		IEngine::Get().GetLogger()->Warning(std::string("Undo not implemented for command: ").append(m_name));
	}
}

void DefaultCommand::ExecuteLater()
{
	const ICommandManagerPtr& commandManager = IEngine::Get().GetCommandManager();
	if (commandManager)
	{
		commandManager->QueueCommand(shared_from_this());
	}
	else
	{
		IEngine::Get().GetLogger()->Error("Command Manager not available");
	}
}

void DefaultCommand::SetCallback(std::function<void()>&& callback)
{
	m_callback = std::move(callback);
}

void DefaultCommand::SetUndoCallback(std::function<void()>&& callback)
{
	m_undoCallback = std::move(callback);
}

void DefaultCommand::SetShortcut(std::string_view shortcut)
{
	m_shortcutStr = std::string(shortcut);
}

void DefaultCommand::SetDescription(std::string_view description)
{
	m_description = std::string(description);
}

std::string_view DefaultCommand::GetShortcutString() const
{
	return m_shortcutStr;
}

void DefaultCommand::Load() {}

} // namespace RCLib::Impl