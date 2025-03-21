#pragma once
#include "IState.h"
#include "RCLib_Fwd.h"

#include <functional>
#include <vector>

namespace RCLib
{

/**
 * @brief Interface for the entity management system
 *
 * Provides functionality for creating and managing game entities.
 * Features:
 * - Unique entity identification
 * - Entity lifecycle management
 * - Thread-safe entity operations
 * - Deferred entity creation and destruction
 */
class RCLIB_API IEntityManager : public IState
{
public:
	/**
	 * @brief Virtual destructor
	 */
	virtual ~IEntityManager() = default;

	/**
	 * @brief Create a new entity
	 *
	 * Creates a new entity with a unique identifier.
	 * The creation may be deferred until the next update.
	 *
	 * @return The unique identifier of the created entity
	 */
	virtual IEntityPtr CreateEntity() = 0;

	/**
	 * @brief Destroy an existing entity
	 *
	 * Marks an entity for destruction. The actual destruction
	 * may be deferred until the next update.
	 *
	 * @param entityId The identifier of the entity to destroy
	 */
	virtual void DestroyEntity(IEntityPtr entity) = 0;

	/**
	 * @brief Check if an entity is valid
	 *
	 * An entity is valid if it has been created and not yet destroyed.
	 *
	 * @param entityId The identifier of the entity to check
	 * @return true if the entity exists and is valid, false otherwise
	 */
	virtual bool IsValid(TEntityId entityId) const = 0;

	virtual IEntityPtr GetEntity(TEntityId id) const = 0;

	virtual std::vector<IEntityPtr> GetAllEntities() const = 0;

	virtual void ForEachEntity(const std::function<void(IEntityPtr)>& func) const = 0;

protected:
	IEntityManager() = default;
};

} // namespace RCLib