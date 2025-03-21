#include "DefaultCommandManager.h"
#include "RCLib.h"

namespace RCLib::Impl
{

DefaultCommandManager::DefaultCommandManager()
{
	try
	{
		// Add basic commands
		ICommandPtr undoCmd = AddCommand("undo");
		undoCmd->SetDescription("Undoes the last executed command");
		undoCmd->SetCallback([this]() { Undo(); });

		ICommandPtr clearCmd = AddCommand("clear_queue");
		clearCmd->SetDescription("Clears the command queue");
		clearCmd->SetCallback([this]() { ClearCommandQueue(); });

		ICommandPtr showQueueCmd = AddCommand("show_queue");
		showQueueCmd->SetDescription("Shows the current command queue size");
		showQueueCmd->SetCallback([this]() { IEngine::Get().GetLogger()->Info("Command queue size: " + std::to_string(GetQueueSize())); });

		m_isLoaded = true;
		IEngine::Get().GetLogger()->Info("Command Manager initialized successfully");
	}
	catch (const std::exception& e)
	{
		IEngine::Get().GetLogger()->Error(std::string("Command Manager initialization failed: ") + e.what());
	}
}

DefaultCommandManager::~DefaultCommandManager()
{
	ClearCommandQueue();
}

bool DefaultCommandManager::OnInitialize()
{
	try
	{
		m_commands.clear();
		m_commandQueue = std::queue<ICommandPtr>();
		m_commandsHistory.clear();
		m_isLoaded = false;
		return true;
	}
	catch (const std::exception& e)
	{
		IEngine::Get().GetLogger()->Error(std::string("Command Manager initialization failed: ") + e.what());
		return false;
	}
}

bool DefaultCommandManager::OnUpdate()
{
	if (!m_isLoaded)
	{
		IEngine::Get().GetLogger()->Error("Cannot update: Command Manager not initialized");
		return false;
	}

	try
	{
		ICommandPtr command;
		{
			std::lock_guard<std::mutex> lock(m_queueMutex);
			if (!m_commandQueue.empty())
			{
				command = m_commandQueue.front();
				m_commandQueue.pop();
			}
		}

		if (command)
		{
			ExecuteCommand(command);
		}
		return true;
	}
	catch (const std::exception& e)
	{
		IEngine::Get().GetLogger()->Error(std::string("Command Manager update failed: ") + e.what());
		return false;
	}
}

void DefaultCommandManager::OnRelease()
{
	ClearCommandQueue();
	std::lock_guard<std::mutex> lock(m_mutex);
	m_commands.clear();
	m_commandsHistory.clear();
}

ICommandPtr DefaultCommandManager::AddCommand(std::string_view name)
{
	if (!m_isLoaded)
	{
		IEngine::Get().GetLogger()->Error("Cannot add command: Manager not initialized");
		return nullptr;
	}

	std::lock_guard<std::mutex> lock(m_mutex);
	auto                        command = IFactory::Get()->Create<ICommand>();
	m_commands[std::string(name)]       = command;
	IEngine::Get().GetLogger()->Info(std::string("Command added: ").append(name));
	return command;
}

bool DefaultCommandManager::ExecuteCommand(std::string_view name)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	auto                        it = m_commands.find(std::string(name));
	if (it != m_commands.end())
	{
		ExecuteCommand(it->second);
		return true;
	}
	IEngine::Get().GetLogger()->Warning(std::string("Command not found: ").append(name));
	return false;
}

void DefaultCommandManager::QueueCommand(ICommandPtr command)
{
	if (!m_isLoaded)
	{
		IEngine::Get().GetLogger()->Error("Cannot queue command: Manager not initialized");
		return;
	}

	if (command)
	{
		std::lock_guard<std::mutex> lock(m_queueMutex);
		m_commandQueue.push(command);
		IEngine::Get().GetLogger()->Debug("Command queued for execution");
	}
}

void DefaultCommandManager::ExecuteCommand(ICommandPtr command)
{
	if (!m_isLoaded)
	{
		IEngine::Get().GetLogger()->Error("Cannot execute command: Manager not initialized");
		return;
	}

	if (command)
	{
		command->Execute();
		std::lock_guard<std::mutex> lock(m_mutex);
		m_commandsHistory.push_back(command);
		IEngine::Get().GetLogger()->Debug("Command executed");
	}
}

void DefaultCommandManager::ClearCommandQueue()
{
	std::lock_guard<std::mutex> lock(m_queueMutex);
	std::queue<ICommandPtr>     empty;
	std::swap(m_commandQueue, empty);
	IEngine::Get().GetLogger()->Info("Command queue cleared");
}

size_t DefaultCommandManager::GetQueueSize() const
{
	std::lock_guard<std::mutex> lock(m_queueMutex);
	return m_commandQueue.size();
}

void DefaultCommandManager::Save()
{
	// TODO: Implement command serialization
}

void DefaultCommandManager::Load()
{
	// TODO: Implement command deserialization
}

bool DefaultCommandManager::Undo()
{
	if (!m_isLoaded)
	{
		IEngine::Get().GetLogger()->Error("Cannot undo: Command Manager not initialized");
		return false;
	}

	std::lock_guard<std::mutex> lock(m_mutex);
	if (m_commandsHistory.empty())
	{
		IEngine::Get().GetLogger()->Warning("No commands to undo");
		return false;
	}

	try
	{
		m_commandsHistory.back()->Undo();
		m_commandsHistory.pop_back();
		IEngine::Get().GetLogger()->Debug("Command undone successfully");
		return true;
	}
	catch (const std::exception& e)
	{
		IEngine::Get().GetLogger()->Error(std::string("Command undo failed: ").append(e.what()));
		return false;
	}
}

ICommandPtr DefaultCommandManager::GetCommand(std::string_view name) const
{
	std::lock_guard<std::mutex> lock(m_mutex);
	auto                        it = m_commands.find(std::string(name));
	if (it != m_commands.end())
	{
		return it->second;
	}
	return nullptr;
}

std::vector<std::string> DefaultCommandManager::GetCommandNames() const
{
	std::lock_guard<std::mutex> lock(m_mutex);
	std::vector<std::string>    names;
	names.reserve(m_commands.size());
	for (const auto& [name, _] : m_commands)
	{
		names.push_back(name);
	}
	return names;
}

} // namespace RCLib::Impl