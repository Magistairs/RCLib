#include "QtLogger.h"
#include "LogWidget.h"

#include <QApplication>
#include <QDateTime>
#include <QEvent>

#include <iostream>
#include <filesystem>

namespace TestsQt
{

QtLogger::QtLogger(LogWidget* logWidget)
  : m_logWidget(logWidget)
{
}

bool QtLogger::OnInitialize()
{
	std::lock_guard<std::mutex> lock(m_logMutex);
	m_isInitialized = true;
	return true;
}

bool QtLogger::OnUpdate()
{
	// Nothing to update
	return true;
}

void QtLogger::OnRelease()
{
	std::lock_guard<std::mutex> lock(m_logMutex);
	if (m_logStream.is_open())
	{
		m_logStream.close();
	}
	m_isInitialized = false;
}

void QtLogger::SetLogWidget(LogWidget* widget)
{
	std::lock_guard<std::mutex> lock(m_logMutex);
	m_logWidget = widget;
}

void QtLogger::Debug(std::string_view message)
{
	if (m_logLevel <= RCLib::ELogLevel::eDebug)
	{
		std::lock_guard<std::mutex> lock(m_logMutex);
		if (m_consoleOutput)
		{
			std::cout << "[DEBUG] " << message << std::endl;
		}
		if (m_fileOutput && m_logStream.is_open())
		{
			m_logStream << "[DEBUG] " << message << std::endl;
		}
		if (m_logWidget)
		{
			QApplication::postEvent(m_logWidget, new QEvent(QEvent::User), Qt::QueuedConnection);
			m_logWidget->AppendLog(QString::fromStdString(std::string(message)), "DEBUG");
		}
	}
}

void QtLogger::Info(std::string_view message)
{
	if (m_logLevel <= RCLib::ELogLevel::eInfo)
	{
		std::lock_guard<std::mutex> lock(m_logMutex);
		if (m_consoleOutput)
		{
			std::cout << "[INFO] " << message << std::endl;
		}
		if (m_fileOutput && m_logStream.is_open())
		{
			m_logStream << "[INFO] " << message << std::endl;
		}
		if (m_logWidget)
		{
			QApplication::postEvent(m_logWidget, new QEvent(QEvent::User), Qt::QueuedConnection);
			m_logWidget->AppendLog(QString::fromStdString(std::string(message)), "INFO");
		}
	}
}

void QtLogger::Warning(std::string_view message)
{
	if (m_logLevel <= RCLib::ELogLevel::eWarning)
	{
		std::lock_guard<std::mutex> lock(m_logMutex);
		if (m_consoleOutput)
		{
			std::cout << "[WARNING] " << message << std::endl;
		}
		if (m_fileOutput && m_logStream.is_open())
		{
			m_logStream << "[WARNING] " << message << std::endl;
		}
		if (m_logWidget)
		{
			QApplication::postEvent(m_logWidget, new QEvent(QEvent::User), Qt::QueuedConnection);
			m_logWidget->AppendLog(QString::fromStdString(std::string(message)), "WARNING");
		}
	}
}

void QtLogger::Error(std::string_view message)
{
	if (m_logLevel <= RCLib::ELogLevel::eError)
	{
		std::lock_guard<std::mutex> lock(m_logMutex);
		if (m_consoleOutput)
		{
			std::cerr << "[ERROR] " << message << std::endl;
		}
		if (m_fileOutput && m_logStream.is_open())
		{
			m_logStream << "[ERROR] " << message << std::endl;
		}
		if (m_logWidget)
		{
			QApplication::postEvent(m_logWidget, new QEvent(QEvent::User), Qt::QueuedConnection);
			m_logWidget->AppendLog(QString::fromStdString(std::string(message)), "ERROR");
		}
	}
}

void QtLogger::SetLogLevel(RCLib::ELogLevel eLevel)
{
	std::lock_guard<std::mutex> lock(m_logMutex);
	m_logLevel = eLevel;
}

void QtLogger::EnableConsoleOutput(bool bEnable)
{
	std::lock_guard<std::mutex> lock(m_logMutex);
	m_consoleOutput = bEnable;
}

void QtLogger::EnableFileOutput(bool bEnable)
{
	std::lock_guard<std::mutex> lock(m_logMutex);
	m_fileOutput = bEnable;
	if (!bEnable && m_logStream.is_open())
	{
		m_logStream.close();
	}
}

void QtLogger::SetLogFile(const std::filesystem::path& filePath)
{
	std::lock_guard<std::mutex> lock(m_logMutex);
	m_logFilePath = filePath;
	if (m_fileOutput && m_logStream.is_open())
	{
		m_logStream.close();
		m_logStream.open(m_logFilePath, std::ios::app);
	}
}

} // namespace TestsQt