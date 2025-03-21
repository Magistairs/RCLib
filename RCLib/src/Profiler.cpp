#include "Profiler.h"
#include "RCLib.h"

#include <stack>

namespace RCLib
{
namespace
{
// Move thread_local stack to file scope
thread_local std::stack<Profiler*> g_activeProfilers;
} // namespace

void Profiler::PushProfiler(Profiler* profiler)
{
	g_activeProfilers.push(profiler);
}

void Profiler::PopProfiler()
{
	if (!g_activeProfilers.empty())
	{
		g_activeProfilers.pop();
	}
}

Profiler* Profiler::GetCurrentProfiler()
{
	return g_activeProfilers.empty() ? nullptr : g_activeProfilers.top();
}

Profiler::Profiler(std::string_view name)
  : m_name(name)
  , m_start(std::chrono::high_resolution_clock::now())
{
	// If there's an active profiler, pause it
	if (Profiler* current = GetCurrentProfiler())
	{
		current->Pause();
	}

	// Add this profiler to the stack
	PushProfiler(this);
}

Profiler::~Profiler()
{
	// Remove this profiler from the stack
	PopProfiler();

	// Resume parent profiler if any
	if (Profiler* current = GetCurrentProfiler())
	{
		current->Resume();
	}

	// Log the final time for this scope
	auto elapsed = GetElapsedMs();
	IEngine::Get().GetLogger()->Debug(m_name + ": " + std::to_string(elapsed) + "ms");
}

double Profiler::GetElapsedMs() const
{
	auto elapsed = m_accumulated;

	// If not paused, add current running time
	if (!m_isPaused)
	{
		auto now = std::chrono::high_resolution_clock::now();
		elapsed += std::chrono::duration_cast<std::chrono::nanoseconds>(now - m_start);
	}

	// Convert to milliseconds
	return elapsed.count() / 1'000'000.0;
}

void Profiler::Pause()
{
	if (!m_isPaused)
	{
		auto now = std::chrono::high_resolution_clock::now();
		m_accumulated += std::chrono::duration_cast<std::chrono::nanoseconds>(now - m_start);
		m_isPaused = true;
	}
}

void Profiler::Resume()
{
	if (m_isPaused)
	{
		m_start    = std::chrono::high_resolution_clock::now();
		m_isPaused = false;
	}
}
} // namespace RCLib
