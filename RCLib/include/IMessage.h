#pragma once
#include "RCLib_Fwd.h"

namespace RCLib
{

/**
 * @brief Base interface for all messages in the system
 */
class RCLIB_API IMessage
{
public:
	virtual ~IMessage() = default;

	/**
	 * @brief Get the type identifier for this message
	 * @return Unique identifier for the message type
	 */
	virtual size_t GetType() const = 0;

	/**
	 * @brief Create a copy of this message
	 * @return Smart pointer to the cloned message
	 */
	virtual SharedPtr<IMessage> Clone() const = 0;

protected:
	IMessage() = default;
};

/**
 * @brief Helper to generate unique message type IDs
 */
template<typename T>
static constexpr TMessageType GetMessageTypeID()
{
	static_assert(std::is_base_of<IMessage, T>::value, "T must inherit from IMessage");
	static const TMessageType type = typeid(T).hash_code();
	return type;
}

} // namespace RCLib