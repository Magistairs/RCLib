#pragma once
#include "IState.h"
#include "RCLib_Fwd.h"

namespace RCLib
{

/**
 * @brief Manager for simple event notifications using integer IDs
 */
class RCLIB_API IEventManager : public IState
{
public:
	virtual ~IEventManager() = default;

	/**
	 * @brief Add a listener for an event
	 * @param owner Owner object for the callback (used for cleanup)
	 * @param eventID Event identifier
	 * @param callback Function to call when the event is triggered
	 */
	virtual void AddEventListener(void* owner, TEventType eventID, FEventCallback callback) = 0;

	/**
	 * @brief Remove a specific event listener
	 * @param owner Owner object that registered the callback
	 * @param eventID Event identifier
	 */
	virtual void RemoveEventListener(void* owner, TEventType eventID) = 0;

	/**
	 * @brief Remove all event listeners for an owner
	 * @param owner Owner object whose listeners should be removed
	 */
	virtual void RemoveAllEventListeners(void* owner) = 0;

	/**
	 * @brief Post an event for processing
	 * @param eventID Event identifier to trigger
	 */
	virtual void PostEvent(TEventType eventID) = 0;

	/**
	 * @brief Process all pending events
	 */
	virtual void ProcessEvents() = 0;

protected:
	IEventManager() = default;
};

} // namespace RCLib