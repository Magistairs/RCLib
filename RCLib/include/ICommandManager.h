#pragma once
#include "IState.h"
#include "RCLib_Fwd.h"

#include <string_view>

namespace RCLib
{

/**
 * @brief Interface for the command management system
 *
 * Provides functionality to register, execute and manage commands in the engine.
 * Commands are executed in a queue-based system, allowing for delayed or immediate execution.
 */
class RCLIB_API ICommandManager : public IState
{
public:
	/**
	 * @brief Virtual destructor
	 */
	virtual ~ICommandManager() = default;

	/**
	 * @brief Adds a command to the execution queue
	 * @param command The command to be executed
	 */
	virtual void QueueCommand(ICommandPtr command) = 0;

	/**
	 * @brief Executes a command immediately, bypassing the queue
	 * @param command The command to execute
	 */
	virtual void ExecuteCommand(ICommandPtr command) = 0;

	/**
	 * @brief Clears all pending commands from the queue
	 */
	virtual void ClearCommandQueue() = 0;

	/**
	 * @brief Gets the number of commands currently in the queue
	 * @return The number of pending commands
	 */
	virtual size_t GetQueueSize() const = 0;

	// Additional command management methods
	virtual ICommandPtr AddCommand(std::string_view name)     = 0;
	virtual bool        ExecuteCommand(std::string_view name) = 0;
	virtual void        Save()                                = 0;
	virtual void        Load()                                = 0;
	virtual bool        Undo()                                = 0;

protected:
	ICommandManager() = default;
};
} // namespace RCLib