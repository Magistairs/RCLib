#pragma once
#include "ILogger.h"
#include "RCLib_Fwd.h"

#include <fstream>
#include <mutex>
#include <string_view>

namespace RCLib::Impl
{
/**
 * @brief Default implementation of the logging system
 * @ingroup Core
 */
class RCLIB_API DefaultLogger : public ILogger
{
public:
	DefaultLogger();
	~DefaultLogger() override;

	// ILogger interface
	void Debug(std::string_view message) override;
	void Info(std::string_view message) override;
	void Warning(std::string_view message) override;
	void Error(std::string_view message) override;
	void SetLogLevel(ELogLevel eLevel) override;
	void EnableConsoleOutput(bool bEnable) override;
	void EnableFileOutput(bool bEnable) override;
	void SetLogFile(const std::filesystem::path& filePath) override;

	// IState interface
	bool OnInitialize() override;
	bool OnUpdate() override;
	void OnRelease() override;

private:
	void        LogMessage(ELogLevel eLevel, std::string_view message);
	std::string GetTimestamp() const;

	ELogLevel          m_logLevel{ELogLevel::eDebug};
	std::ofstream      m_logFile;
	bool               m_consoleEnabled{true};
	bool               m_fileEnabled{false};
	mutable std::mutex m_mutex;
};

} // namespace RCLib::Impl