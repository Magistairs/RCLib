#pragma once
#include "RCLib_Fwd.h"

namespace RCLib
{

/**
 * @brief Base interface for all events in the system
 */
class RCLIB_API IEvent
{
public:
	virtual ~IEvent() = default;

	/**
	 * @brief Get the type identifier for this event
	 * @return Unique identifier for the event type
	 */
	virtual TEventType GetType() const = 0;

protected:
	IEvent() = default;
};

/**
 * @brief Helper to generate unique event type IDs
 */
template<typename T>
static constexpr TEventType GetEventTypeID()
{
	static_assert(std::is_base_of<IEvent, T>::value, "T must inherit from IEvent");
	static const TEventType type = typeid(T).hash_code();
	return type;
}

} // namespace RCLib