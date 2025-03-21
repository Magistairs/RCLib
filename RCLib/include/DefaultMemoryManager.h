#pragma once
#include "IMemoryManager.h"
#include "RCLib_Fwd.h"

#include <atomic>
#include <chrono>
#include <mutex>
#include <unordered_map>

namespace RCLib::Impl
{
/**
 * @brief Thread-safe implementation of the memory manager
 * @ingroup Core
 */
class RCLIB_API DefaultMemoryManager : public IMemoryManager
{
public:
	explicit DefaultMemoryManager();
	~DefaultMemoryManager() override;

	// IMemoryManager interface implementation
	void   TrackAllocation(void* ptr, size_t size, const char* file, int line) override;
	void   TrackDeallocation(void* ptr) override;
	size_t GetCurrentAllocatedBytes() const override;
	size_t GetCurrentAllocationCount() const override;
	void   ReportMemoryState(bool bDetailed = false) const override;
	bool   CheckForLeaks() const override;
	void   EnableTracking(bool bEnable) override;

	// Managed interface implementation
	bool OnInitialize() override;
	bool OnUpdate() override;
	void OnRelease() override;

private:
	// Mutex for thread safety
	mutable std::mutex m_memoryMutex;

	// Memory tracking
	std::atomic<size_t> m_totalAllocated{0};
	std::atomic<size_t> m_totalFreed{0};
	std::atomic<size_t> m_allocationCount{0};
	std::atomic<bool>   m_trackingEnabled{true};

	struct AllocationInfo
	{
		size_t                                size;
		const char*                           file;
		int                                   line;
		std::chrono::steady_clock::time_point timestamp;
	};

	std::unordered_map<void*, AllocationInfo> m_allocations;
};

} // namespace RCLib::Impl