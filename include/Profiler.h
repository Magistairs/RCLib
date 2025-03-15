#pragma once

#include "DLLExport.h"

#define PROFILE_SCOPE(name) RCLib::Profiler __profiler(name)
#define PROFILE_FUNCTION    RCLib::Profiler __functionProfiler(__FUNCTION__);

namespace RCLib
{
	class RCLIB_API Profiler
	{
	public:
		explicit Profiler(const char* name);
		~Profiler();

		void Start();
		void Pause();
		void Resume();

	private:
		const char* m_name;
		//sf::Time    m_timer;
		//sf::Clock   m_clock;
		Profiler* m_pParent{ nullptr };

		inline static Profiler* s_pLastProfiler{ nullptr };
	};
} // namespace RCLib
