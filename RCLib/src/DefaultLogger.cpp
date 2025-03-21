#include "DefaultLogger.h"
#include "RCLib.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <unordered_map>

namespace RCLib::Impl
{

DefaultLogger::DefaultLogger() = default;

DefaultLogger::~DefaultLogger() = default;

bool DefaultLogger::OnInitialize()
{
	try
	{
		Info("Logger initialized");
		return true;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Logger initialization failed: " << e.what() << std::endl;
		return false;
	}
}

bool DefaultLogger::OnUpdate()
{
	return true;
}

void DefaultLogger::OnRelease()
{
	try
	{
		Info("Logger shutting down");
		if (m_logFile.is_open())
		{
			m_logFile.close();
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Logger release failed: " << e.what() << std::endl;
	}
}

void DefaultLogger::Debug(std::string_view message)
{
	if (m_logLevel <= ELogLevel::eDebug)
	{
		LogMessage(ELogLevel::eDebug, message);
	}
}

void DefaultLogger::Info(std::string_view message)
{
	if (m_logLevel <= ELogLevel::eInfo)
	{
		LogMessage(ELogLevel::eInfo, message);
	}
}

void DefaultLogger::Warning(std::string_view message)
{
	if (m_logLevel <= ELogLevel::eWarning)
	{
		LogMessage(ELogLevel::eWarning, message);
	}
}

void DefaultLogger::Error(std::string_view message)
{
	if (m_logLevel <= ELogLevel::eError)
	{
		LogMessage(ELogLevel::eError, message);
	}
}

void DefaultLogger::SetLogLevel(ELogLevel eLevel)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_logLevel = eLevel;
}

void DefaultLogger::EnableConsoleOutput(bool bEnable)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_consoleEnabled = bEnable;
}

void DefaultLogger::EnableFileOutput(bool bEnable)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_fileEnabled = bEnable;
}

void DefaultLogger::SetLogFile(const std::filesystem::path& filePath)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	if (m_logFile.is_open())
	{
		m_logFile.close();
	}

	try
	{
		m_logFile.open(filePath, std::ios::out | std::ios::app);
		if (!m_logFile)
		{
			std::cerr << "Failed to open log file: " << filePath << std::endl;
		}
		else
		{
			m_fileEnabled = true;
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Failed to open log file: " << e.what() << std::endl;
	}
}

void DefaultLogger::LogMessage(ELogLevel eLevel, std::string_view message)
{
	static const std::unordered_map<ELogLevel, std::string_view> levelStrings = {{ELogLevel::eDebug, "DEBUG"},
	  {ELogLevel::eInfo, "INFO"},
	  {ELogLevel::eWarning, "WARNING"},
	  {ELogLevel::eError, "ERROR"}};

	try
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		std::string                 formattedMessage = GetTimestamp() + " [" + std::string(levelStrings.at(eLevel)) + "] " + std::string(message) + "\n";

		// Write to console if enabled
		if (m_consoleEnabled)
		{
			std::cout << formattedMessage;
		}

		// Write to file if enabled and open
		if (m_fileEnabled && m_logFile.is_open())
		{
			m_logFile << formattedMessage;
			m_logFile.flush();
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Logging failed: " << e.what() << std::endl;
	}
}

std::string DefaultLogger::GetTimestamp() const
{
	auto now  = std::chrono::system_clock::now();
	auto time = std::chrono::system_clock::to_time_t(now);
	auto ms   = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

	std::tm timeinfo;
	localtime_s(&timeinfo, &time);

	std::stringstream ss;
	ss << std::put_time(&timeinfo, "%Y-%m-%d %H:%M:%S");
	ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
	return ss.str();
}

} // namespace RCLib::Impl