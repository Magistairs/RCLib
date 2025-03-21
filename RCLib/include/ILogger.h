#pragma once
#include "IState.h"
#include "RCLib_Fwd.h"

#include <filesystem>
#include <string_view>

namespace RCLib
{

/**
 * @brief Logging levels for message categorization
 *
 * Defines the severity levels for log messages:
 * - Debug: Detailed information for debugging
 * - Info: General information about normal operation
 * - Warning: Potential issues that don't affect core functionality
 * - Error: Serious issues that affect functionality
 */
enum class ELogLevel
{
	eDebug,
	eInfo,
	eWarning,
	eError
};

/**
 * @brief Interface for the logging system
 *
 * Provides logging functionality with different severity levels.
 * Supports both console and file output with configurable settings.
 * Thread-safe logging operations for concurrent access.
 */
class RCLIB_API ILogger : public IState
{
public:
	/**
	 * @brief Virtual destructor
	 */
	virtual ~ILogger() = default;

	/**
	 * @brief Log a debug message
	 * @param message The message to log
	 */
	virtual void Debug(std::string_view message) = 0;

	/**
	 * @brief Log an informational message
	 * @param message The message to log
	 */
	virtual void Info(std::string_view message) = 0;

	/**
	 * @brief Log a warning message
	 * @param message The message to log
	 */
	virtual void Warning(std::string_view message) = 0;

	/**
	 * @brief Log an error message
	 * @param message The message to log
	 */
	virtual void Error(std::string_view message) = 0;

	/**
	 * @brief Set the minimum log level
	 * Messages below this level will be ignored
	 * @param level The minimum log level to display
	 */
	virtual void SetLogLevel(ELogLevel eLevel) = 0;

	/**
	 * @brief Enable or disable console output
	 * @param enable True to enable console output, false to disable
	 */
	virtual void EnableConsoleOutput(bool bEnable) = 0;

	/**
	 * @brief Enable or disable file output
	 * @param enable True to enable file output, false to disable
	 */
	virtual void EnableFileOutput(bool bEnable) = 0;

	/**
	 * @brief Set the output file for logging
	 * @param filePath Path to the log file
	 */
	virtual void SetLogFile(const std::filesystem::path& filePath) = 0;

protected:
	ILogger() = default;
};

} // namespace RCLib