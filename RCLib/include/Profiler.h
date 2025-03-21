#pragma once

#include "DLLExport.h"

#include <chrono>
#include <string>

#define PROFILE_SCOPE(name) RCLib::Profiler __profiler(name)
#define PROFILE_FUNCTION    RCLib::Profiler __functionProfiler(__FUNCTION__);

namespace RCLib
{
// RAII profiler that handles nested scopes without affecting parent timing
class RCLIB_API Profiler
{
public:
	// Creates a new profiling scope. Parent scope's timer is paused until this scope ends
	explicit Profiler(std::string_view name);
	~Profiler();

	// Get elapsed time in milliseconds for current scope
	double GetElapsedMs() const;

	// Disable copy/move to ensure RAII behavior
	Profiler(const Profiler&)            = delete;
	Profiler& operator=(const Profiler&) = delete;
	Profiler(Profiler&&)                 = delete;
	Profiler& operator=(Profiler&&)      = delete;

private:
	// Pause current scope's timer
	void Pause();
	// Resume current scope's timer
	void Resume();

	std::string                                    m_name;
	std::chrono::high_resolution_clock::time_point m_start;
	std::chrono::nanoseconds                       m_accumulated{0};
	bool                                           m_isPaused{false};

	// Static methods to manage the active profiler stack
	static void      PushProfiler(Profiler* profiler);
	static void      PopProfiler();
	static Profiler* GetCurrentProfiler();
};
} // namespace RCLib
