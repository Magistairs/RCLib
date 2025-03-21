#pragma once
#include "LogWidget.h"

#include <RCLib.h>

#include <fstream>
#include <mutex>

namespace TestsQt
{
/**
 * @brief Qt-based logger implementation that displays logs in a widget
 */
class QtLogger : public RCLib::ILogger
{
public:
	explicit QtLogger(LogWidget* logWidget = nullptr);
	~QtLogger() override = default;

	// ILogger interface implementation
	void Debug(std::string_view message) override;
	void Info(std::string_view message) override;
	void Warning(std::string_view message) override;
	void Error(std::string_view message) override;
	void SetLogLevel(RCLib::LogLevel level) override;
	void EnableConsoleOutput(bool enable) override;
	void EnableFileOutput(bool enable) override;
	void SetLogFile(const std::filesystem::path& filePath) override;

	// IState interface implementation
	void OnInitialize() override;
	void OnUpdate() override;
	void OnRelease() override;

	// Qt-specific methods
	void       SetLogWidget(LogWidget* widget);
	LogWidget* GetLogWidget() const { return m_logWidget; }

private:
	bool                  m_isInitialized{false};
	bool                  m_consoleOutput{true};
	bool                  m_fileOutput{false};
	RCLib::LogLevel       m_logLevel{RCLib::LogLevel::Info};
	std::filesystem::path m_logFilePath;
	std::ofstream         m_logStream;
	mutable std::mutex    m_logMutex;
	LogWidget*            m_logWidget{nullptr};
};

} // namespace TestsQt