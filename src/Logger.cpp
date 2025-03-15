#include "Logger.h"

#include <assert.h>
#include <iostream>

using namespace RCLib;

Logger* s_pLogger = nullptr;

Logger& Logger::Get()
{
	assert(s_pLogger);
	return *s_pLogger;
}
Logger::Logger()
{
	assert(!s_pLogger);
	s_pLogger = this;
}

Logger::~Logger()
{
	s_pLogger = nullptr;
}

void Logger::Log(ELogLevel level, const std::string& message)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	switch (level)
	{
	case ELogLevel::eInfo:
	{
		std::cout << "INFO: " << message << std::endl;
	}
	break;
	case ELogLevel::eWarning:
	{
		std::cout << "WARNING: " << message << std::endl;
	}
	break;
	case ELogLevel::eError:
	{
		std::cout << "ERROR: " << message << std::endl;
	}
	break;
	}
}
