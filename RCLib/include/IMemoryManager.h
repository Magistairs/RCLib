#pragma once
#include "IState.h"
#include "RCLib_Fwd.h"

namespace RCLib
{

/**
 * @brief Interface for memory management and tracking
 */
class RCLIB_API IMemoryManager : public IState
{
public:
	virtual ~IMemoryManager() = default;

	/**
	 * @brief Track memory allocation
	 */
	virtual void TrackAllocation(void* ptr, size_t size, const char* file, int line) = 0;

	/**
	 * @brief Track memory deallocation
	 */
	virtual void TrackDeallocation(void* ptr) = 0;

	/**
	 * @brief Get total allocated bytes
	 */
	virtual size_t GetCurrentAllocatedBytes() const = 0;

	/**
	 * @brief Get current allocation count
	 */
	virtual size_t GetCurrentAllocationCount() const = 0;

	/**
	 * @brief Report current memory state
	 */
	virtual void ReportMemoryState(bool detailed = false) const = 0;

	/**
	 * @brief Check for memory leaks
	 */
	virtual bool CheckForLeaks() const = 0;

	/**
	 * @brief Enable/disable memory tracking
	 */
	virtual void EnableTracking(bool enable) = 0;

protected:
	IMemoryManager() = default;
};

} // namespace RCLib