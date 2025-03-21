#include "DefaultMemoryManager.h"
#include "RCLib.h"

namespace RCLib::Impl
{

DefaultMemoryManager::DefaultMemoryManager() {}

DefaultMemoryManager::~DefaultMemoryManager()
{
	if (CheckForLeaks())
	{
		ReportMemoryState(true);
	}
}

void DefaultMemoryManager::TrackAllocation(void* ptr, size_t size, const char* file, int line)
{
	if (!m_trackingEnabled)
		return;

	std::lock_guard<std::mutex> lock(m_memoryMutex);
	m_totalAllocated += size;
	m_allocationCount++;

	AllocationInfo info;
	info.size      = size;
	info.file      = file;
	info.line      = line;
	info.timestamp = std::chrono::steady_clock::now();

	m_allocations[ptr] = info;
}

void DefaultMemoryManager::TrackDeallocation(void* ptr)
{
	if (!m_trackingEnabled)
		return;

	std::lock_guard<std::mutex> lock(m_memoryMutex);
	auto                        it = m_allocations.find(ptr);
	if (it != m_allocations.end())
	{
		m_totalFreed += it->second.size;
		m_allocationCount--;
		m_allocations.erase(it);
	}
}

size_t DefaultMemoryManager::GetCurrentAllocatedBytes() const
{
	std::lock_guard<std::mutex> lock(m_memoryMutex);
	return m_totalAllocated - m_totalFreed;
}

size_t DefaultMemoryManager::GetCurrentAllocationCount() const
{
	std::lock_guard<std::mutex> lock(m_memoryMutex);
	return m_allocationCount;
}

void DefaultMemoryManager::ReportMemoryState(bool bDetailed) const
{
	std::lock_guard<std::mutex> lock(m_memoryMutex);
	const size_t                currentAllocated = m_totalAllocated - m_totalFreed;

	ILoggerPtr logger = IEngine::Get().GetLogger();
	if (logger)
	{
		logger->Info("Memory State Report:");
		logger->Info("Total Allocated: " + std::to_string(currentAllocated) + " bytes");
		logger->Info("Current Allocation Count: " + std::to_string(m_allocationCount));

		if (bDetailed)
		{
			logger->Info("Detailed Allocations:");
			for (const auto& [ptr, info] : m_allocations)
			{
				logger->Info("Address: " + std::to_string(reinterpret_cast<uintptr_t>(ptr)) + ", Size: " + std::to_string(info.size)
				             + ", File: " + std::string(info.file) + ", Line: " + std::to_string(info.line));
			}
		}
	}
}

bool DefaultMemoryManager::CheckForLeaks() const
{
	std::lock_guard<std::mutex> lock(m_memoryMutex);
	return m_allocationCount > 0;
}

void DefaultMemoryManager::EnableTracking(bool bEnable)
{
	m_trackingEnabled = bEnable;
}

bool DefaultMemoryManager::OnInitialize()
{
	try
	{
		m_totalAllocated  = 0;
		m_totalFreed      = 0;
		m_allocationCount = 0;
		m_trackingEnabled = true;
		m_allocations.clear();

		// Add basic commands
		ICommandManagerPtr commandManager = IEngine::Get().GetCommandManager();

		ICommandPtr reportCmd = commandManager->AddCommand("memory_report");
		reportCmd->SetDescription("Reports current memory state");
		reportCmd->SetCallback([this]() { ReportMemoryState(true); });

		ICommandPtr checkLeaksCmd = commandManager->AddCommand("memory_check_leaks");
		checkLeaksCmd->SetDescription("Checks for memory leaks");
		checkLeaksCmd->SetCallback([this]() {
			if (CheckForLeaks())
				IEngine::Get().GetLogger()->Warning("Memory leaks detected!");
			else
				IEngine::Get().GetLogger()->Info("No memory leaks detected.");
		});

		ICommandPtr toggleTrackingCmd = commandManager->AddCommand("memory_toggle_tracking");
		toggleTrackingCmd->SetDescription("Toggles memory tracking on/off");
		toggleTrackingCmd->SetCallback([this]() {
			bool bNewState = !m_trackingEnabled;
			EnableTracking(bNewState);
			IEngine::Get().GetLogger()->Info(std::string("Memory tracking ") + (bNewState ? "enabled" : "disabled"));
		});

		IEngine::Get().GetLogger()->Info("Memory Manager initialized successfully");
		return true;
	}
	catch (const std::exception& e)
	{
		IEngine::Get().GetLogger()->Error(std::string("Memory Manager initialization failed: ") + e.what());
		return false;
	}
}

bool DefaultMemoryManager::OnUpdate()
{
	try
	{
		// Vérification périodique des fuites mémoire
		if (CheckForLeaks())
		{
			IEngine::Get().GetLogger()->Warning("Memory leaks detected during update");
			ReportMemoryState(true);
		}
		return true;
	}
	catch (const std::exception& e)
	{
		IEngine::Get().GetLogger()->Error(std::string("Memory Manager update failed: ") + e.what());
		return false;
	}
}

void DefaultMemoryManager::OnRelease()
{
	if (CheckForLeaks())
	{
		ReportMemoryState(true);
	}
	m_allocations.clear();
}

} // namespace RCLib::Impl