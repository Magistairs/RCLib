#pragma once
#include "IState.h"
#include "RCLib_Fwd.h"

namespace RCLib
{

/**
 * @brief Pure abstract interface for the message manager
 */
class RCLIB_API IMessageManager : public IState
{
public:
	virtual ~IMessageManager() = default;

	/**
	 * @brief Register a handler for a specific message type
	 * @param type Message type identifier
	 * @param handler Function to call when a message is received
	 */
	virtual void RegisterMessageHandler(TMessageType type, FMessageHandler handler) = 0;

	/**
	 * @brief Post a message to all registered receivers
	 * @param message The message to post
	 */
	virtual void PostMessage(const IMessage& message) = 0;

	/**
	 * @brief Process all pending messages
	 */
	virtual void ProcessMessages() = 0;

protected:
	IMessageManager() = default;
};

} // namespace RCLib