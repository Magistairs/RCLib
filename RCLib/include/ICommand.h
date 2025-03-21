#pragma once

#include "RCLib_Fwd.h"

#include <string_view>

namespace RCLib
{
/**
 * @brief Interface for commands in the command pattern
 *
 * Base interface for all commands that can be executed and undone.
 * Implements the Command pattern for action encapsulation.
 */
class RCLIB_API ICommand
{
public:
	virtual ~ICommand() = default;

	/**
	 * @brief Execute the command
	 */
	virtual void Execute() = 0;

	/**
	 * @brief Undo the command
	 */
	virtual void Undo() = 0;

	/**
	 * @brief Queue the command for later execution
	 */
	virtual void ExecuteLater() = 0;

	/**
	 * @brief Set the command's callback function
	 */
	virtual void SetCallback(std::function<void()>&& callback) = 0;

	/**
	 * @brief Set the command's description
	 */
	virtual void SetDescription(std::string_view description) = 0;

	/**
	 * @brief Set the command's shortcut
	 */
	virtual void SetShortcut(std::string_view shortcut) = 0;

	/**
	 * @brief Get the command's name
	 */
	virtual std::string_view GetName() const = 0;

	/**
	 * @brief Get the command's description
	 */
	virtual std::string_view GetDescription() const = 0;

	/**
	 * @brief Get the command's shortcut string
	 */
	virtual std::string_view GetShortcutString() const = 0;
};

} // namespace RCLib