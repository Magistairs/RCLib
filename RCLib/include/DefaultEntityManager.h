#pragma once
#include "IEntityManager.h"
#include "RCLib_Fwd.h"

#include <atomic>
#include <mutex>
#include <unordered_map>
#include <vector>

namespace RCLib::Impl
{

/**
 * @brief Implementation of the entity management system
 * @ingroup Core
 *
 * @details Thread-safe implementation of the entity system that provides:
 * - Entity creation and destruction
 * - Entity lifecycle management
 * - Entity lookup and validation
 * - Deferred entity operations
 *
 * Features:
 * - Thread-safe entity operations
 * - Deferred entity creation/destruction
 * - Unique entity identification
 * - Entity validation
 *
 * @note All entity operations are thread-safe and can be called from any thread
 * @see IEntity
 * @see EntityImpl
 */
class RCLIB_API DefaultEntityManager : public IEntityManager
{
public:
	explicit DefaultEntityManager();
	~DefaultEntityManager() override = default;

	// IEntityManager interface implementation
	/**
	 * @brief Creates a new entity
	 * @return Smart pointer to the created entity
	 * @thread_safety Thread safe
	 */
	IEntityPtr CreateEntity() override;

	/**
	 * @brief Destroys an existing entity
	 * @param entity The entity to destroy
	 * @thread_safety Thread safe
	 */
	void DestroyEntity(IEntityPtr entity) override;

	/**
	 * @brief Checks if an entity is valid
	 * @param entityId The ID of the entity to check
	 * @return true if the entity exists and is valid, false otherwise
	 * @thread_safety Thread safe
	 */
	bool IsValid(TEntityId entityId) const override;

	/**
	 * @brief Gets an entity by its ID
	 * @param entityId The ID of the entity to get
	 * @return Smart pointer to the entity if found, nullptr otherwise
	 * @thread_safety Thread safe
	 */
	IEntityPtr GetEntity(TEntityId entityId) const override;

	/**
	 * @brief Get all entities
	 * @return A vector of all entities
	 * @thread_safety Thread safe
	 */
	std::vector<IEntityPtr> GetAllEntities() const override;

	/**
	 * @brief Executes a function for each entity
	 * @param func The function to execute
	 * @thread_safety Thread safe
	 */
	void ForEachEntity(const std::function<void(IEntityPtr)>& func) const override;

	// Managed interface implementation
	bool OnInitialize() override;
	bool OnUpdate() override;
	void OnRelease() override;

private:
	/**
	 * @brief Processes pending entity operations
	 * @details Executes all pending entity operations in a thread-safe manner
	 */
	void ProcessPendingOperations();

	std::unordered_map<TEntityId, IEntityPtr> m_entities;
	std::vector<std::function<void()>>        m_pendingOperations;
	mutable std::mutex                        m_entityMutex;
	mutable std::mutex                        m_queueMutex;
	std::atomic<TEntityId>                    m_nextEntityId{1}; // Start from 1 as 0 is InvalidEntityId
};

} // namespace RCLib::Impl