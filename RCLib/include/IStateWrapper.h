#pragma once
#include "IException.h"
#include "IState.h"
#include "RCLib_Fwd.h"

#include <string>
#include <string_view>

namespace RCLib
{
/**
 * @brief Exception thrown when attempting to use an uninitialized state
 */
class RCLIB_API StateNotInitializedException : public IException
{
public:
	StateNotInitializedException()
	  : m_message("State not initialized")
	{
	}
	virtual ~StateNotInitializedException() = default;

	const char* what() const noexcept override { return m_message.c_str(); }

private:
	std::string m_message;
};

/**
 * @brief Interface for state wrapper that manages initialization and lifecycle
 */
class RCLIB_API IStateWrapper
{
public:
	virtual ~IStateWrapper() = default;

	/**
	 * @brief Initialize the manager
	 * @throws ManagerNotInitializedException if dependencies are not initialized
	 */
	virtual void Initialize() = 0;

	/**
	 * @brief Update the manager state
	 * @throws ManagerNotInitializedException if not initialized
	 */
	virtual void Update() = 0;

	/**
	 * @brief Release the manager resources
	 */
	virtual void Release() = 0;

	/**
	 * @brief Check if the manager is initialized
	 * @return true if initialized, false otherwise
	 */
	virtual bool IsInitialized() const = 0;

	/**
	 * @brief Get the manager name
	 * @return The name of the manager
	 */
	virtual std::string_view GetName() const = 0;

	/**
	 * @brief Enable or disable logging
	 * @param enable Whether to enable logging
	 */
	virtual void SetLoggingEnabled(bool enable) = 0;
};

} // namespace RCLib