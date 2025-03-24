#include "DefaultMessageManager.h"
#include "RCLib.h"

namespace RCLib::Impl
{

DefaultMessageManager::DefaultMessageManager() {}

bool DefaultMessageManager::OnInitialize()
{
	try
	{
		// Initialisation de base
		m_messageHandlers.clear();
		while (!m_pendingMessages.empty())
		{
			m_pendingMessages.pop();
		}

		// Add basic commands
		auto commandManager = IEngine::Get().GetCommandManager();

		ICommandPtr showQueueCmd = commandManager->AddCommand("message_show_queue");
		showQueueCmd->SetDescription("Shows the current message queue size");
		showQueueCmd->SetCallback([this]() {
			std::lock_guard<std::mutex> lock(m_pendingMessagesMutex);
			IEngine::Get().GetLogger()->Info("Pending messages: " + std::to_string(m_pendingMessages.size()));
		});

		ICommandPtr clearQueueCmd = commandManager->AddCommand("message_clear_queue");
		clearQueueCmd->SetDescription("Clears all pending messages");
		clearQueueCmd->SetCallback([this]() {
			std::lock_guard<std::mutex> lock(m_pendingMessagesMutex);
			while (!m_pendingMessages.empty())
			{
				m_pendingMessages.pop();
			}
			IEngine::Get().GetLogger()->Info("Message queue cleared");
		});

		IEngine::Get().GetLogger()->Info("Message Manager initialized successfully");
	}
	catch (const std::exception& e)
	{
		IEngine::Get().GetLogger()->Error(std::string("Message Manager initialization failed: ") + e.what());
	}
	return true;
}

bool DefaultMessageManager::OnUpdate()
{
	try
	{
		ProcessMessages();
		return true;
	}
	catch (const std::exception& e)
	{
		IEngine::Get().GetLogger()->Error(std::string("Message Manager update failed: ") + e.what());
		return false;
	}
}

void DefaultMessageManager::OnRelease()
{
	IEngine::Get().GetLogger()->Info("Message Manager releasing...");
	std::unique_lock<std::shared_mutex> lock(m_messageHandlersMutex);
	std::unique_lock<std::mutex>        lock2(m_pendingMessagesMutex);

	m_messageHandlers.clear();
	while (!m_pendingMessages.empty())
	{
		m_pendingMessages.pop();
	}

	IEngine::Get().GetLogger()->Debug("Message Manager released");
}

void DefaultMessageManager::RegisterMessageHandler(TMessageType type, FMessageHandler handler)
{
	std::unique_lock<std::shared_mutex> lock(m_messageHandlersMutex);
	m_messageHandlers[type].push_back(std::move(handler));
	IEngine::Get().GetLogger()->Debug("Message handler registered for type: " + std::to_string(type));
}

void DefaultMessageManager::PostMessage(const IMessage& message)
{
	std::lock_guard<std::mutex> lock(m_pendingMessagesMutex);
	m_pendingMessages.push(message.Clone());
}

void DefaultMessageManager::ProcessMessages()
{
	// Get pending messages
	std::vector<IMessagePtr> messagesToProcess;
	{
		std::unique_lock<std::mutex> lock(m_pendingMessagesMutex);
		while (!m_pendingMessages.empty())
		{
			messagesToProcess.push_back(std::move(m_pendingMessages.front()));
			m_pendingMessages.pop();
		}
	}

	// Process messages
	for (const auto& message : messagesToProcess)
	{
		std::shared_lock<std::shared_mutex> lock(m_messageHandlersMutex);
		auto                                it = m_messageHandlers.find(message->GetType());
		if (it != m_messageHandlers.end())
		{
			for (const auto& handler : it->second)
			{
				try
				{
					handler(*message);
				}
				catch (const std::exception& e)
				{
					IEngine::Get().GetLogger()->Error("Message handler execution failed: " + std::string(e.what()));
				}
			}
		}
		else
		{
			IEngine::Get().GetLogger()->Warning("No handlers registered for message type: " + std::to_string(message->GetType()));
		}
	}
}

} // namespace RCLib::Impl