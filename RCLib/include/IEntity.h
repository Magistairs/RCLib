#pragma once
#include "IState.h"
#include "RCLib_Fwd.h"

#include <string_view>

namespace RCLib
{
/**
 * @brief Base interface for all entities in the system
 */
class RCLIB_API IEntity : public IState
{
public:
	virtual ~IEntity() = default;

	/**
	 * @brief Get the unique identifier for this entity
	 * @return The entity's unique ID
	 */
	virtual TEntityId GetId() const = 0;

	/**
	 * @brief Get the name of the entity
	 * @return The entity's name
	 */
	virtual std::string_view GetName() const = 0;

	/**
	 * @brief Set the name of the entity
	 * @param name The new name for the entity
	 */
	virtual void SetName(std::string_view name) = 0;

	/**
	 * @brief Check if the entity is valid
	 * @return true if the entity is valid, false otherwise
	 */
	virtual bool IsValid() const = 0;

protected:
	IEntity() = default;
};

} // namespace RCLib