#pragma once
#include "DLLExport.h"

#include <mutex>

namespace RCLib
{
	class RCLIB_API Logger
	{
	public:
		enum ELogLevel
		{
			eInfo,
			eWarning,
			eError
		};
		static Logger& Get();
		Logger();
		~Logger();

		void Log(ELogLevel level, const std::string& message);

	private:
		std::mutex m_mutex;
	};
} // namespace RCLib