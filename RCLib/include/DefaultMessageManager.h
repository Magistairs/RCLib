#pragma once
#include "IMessage.h"
#include "IMessageManager.h"
#include "RCLib_Fwd.h"

#include <mutex>
#include <queue>
#include <shared_mutex>
#include <unordered_map>

namespace RCLib::Impl
{

/**
 * @brief Thread-safe implementation of the message manager
 */
class RCLIB_API DefaultMessageManager : public IMessageManager
{
public:
	explicit DefaultMessageManager();
	~DefaultMessageManager() override = default;

	/**
	 * @brief Register a handler for a specific message type
	 * @tparam T Message type (must inherit from IMessage)
	 * @param handler Function to call when a message of type T is received
	 */
	template<typename T>
	void RegisterMessageHandler(std::function<void(const T&)> handler)
	{
		static_assert(std::is_base_of<IMessage, T>::value, "T must inherit from IMessage");
		RegisterMessageHandler(RCLib::GetMessageTypeID<T>(), [handler](const IMessage& msg) { handler(static_cast<const T&>(msg)); });
	}

	/**
	 * @brief Post a message to all registered receivers
	 * @tparam T Message type (must inherit from IMessage)
	 * @param message The message to post
	 */
	template<typename T>
	void PostMessage(const T& message)
	{
		static_assert(std::is_base_of<IMessage, T>::value, "T must inherit from IMessage");
		PostMessage(static_cast<const IMessage&>(message));
	}

	// IMessageManager interface implementation
	void RegisterMessageHandler(TMessageType type, FMessageHandler handler) override;
	void PostMessage(const IMessage& message) override;
	void ProcessMessages() override;

	// IState interface implementation
	bool OnInitialize() override;
	bool OnUpdate() override;
	void OnRelease() override;

private:
	// Mutexes for thread safety
	mutable std::shared_mutex m_messageHandlersMutex;
	mutable std::mutex        m_pendingMessagesMutex;

	// Message handlers and queue
	std::unordered_map<TMessageType, std::vector<FMessageHandler>> m_messageHandlers;
	std::queue<IMessagePtr>                                        m_pendingMessages;
};

} // namespace RCLib::Impl