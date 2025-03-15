#pragma once
#include "DLLExport.h"

#include <iostream>
#include <memory>
#include <shared_mutex>
#include <unordered_map>

// Macro to create a tracked shared_ptr
#ifdef _DEBUG
#define make_shared(type, ...) RCLib::Memory::make_tracked_shared<type>(__FILE__, __LINE__, ##__VA_ARGS__)
#else
#define make_shared(type, ...) std::make_shared<type>(##__VA_ARGS__)
#endif

namespace RCLib
{
	namespace Memory
	{
		namespace priv
		{
			// Structure to hold allocation information
			struct AllocationInfo
			{
				size_t      size;
				const char* file;
				int         line;
			};

			// Global map to store allocation information
			inline std::unordered_map<void*, AllocationInfo> allocations;
			inline std::shared_mutex                         allocationsMutex;
			inline size_t                                    totalAllocated = 0;
			inline size_t                                    totalFreed = 0;

			// Track an allocation
			inline void trackAllocation(void* ptr, size_t size, const char* file, int line)
			{
				std::lock_guard<std::shared_mutex> lock(allocationsMutex);
				allocations[ptr] = { size, file, line };
				totalAllocated += size;
			}

			// Track a deallocation
			inline void trackDeallocation(void* ptr)
			{
				std::lock_guard<std::shared_mutex> lock(allocationsMutex);

				auto it = allocations.find(ptr);
				if (it != allocations.end())
				{
					totalFreed += it->second.size;
					allocations.erase(it);
				}
			}
			// Custom deleter to track deallocations
			template<typename T>
			struct TrackingDeleter
			{
				void operator()(T* ptr) const
				{
					trackDeallocation(ptr);
					delete ptr;
				}
			};
		} // namespace priv

		inline void Report()
		{
			std::shared_lock<std::shared_mutex> lock(priv::allocationsMutex);
			std::cout << "Currently allocated: " << priv::totalAllocated - priv::totalFreed << " bytes\n";
		}

		inline void ReportLeaks()
		{
			std::shared_lock<std::shared_mutex> lock(priv::allocationsMutex);
			std::cout << "Total allocated: " << priv::totalAllocated << " bytes\n";
			std::cout << "Total freed: " << priv::totalFreed << " bytes\n";
			if (!priv::allocations.empty())
			{
				std::cout << "Memory leaks detected:\n";
				for (const auto& pair : priv::allocations)
				{
					std::cout << "Leaked " << pair.second.size << " bytes at " << pair.first << " (allocated in " << pair.second.file << ":" << pair.second.line
						<< ")\n";
				}
			}
		}

		// Function to create a tracked shared_ptr
		template<typename T, typename... Args>
		std::shared_ptr<T> make_tracked_shared(const char* file, int line, Args&&... args)
		{
			T* rawPtr = new T(std::forward<Args>(args)...);
			priv::trackAllocation(rawPtr, sizeof(T), file, line);
			return std::shared_ptr<T>(rawPtr, priv::TrackingDeleter<T>());
		}
	} // namespace Memory

} // namespace RCLib
