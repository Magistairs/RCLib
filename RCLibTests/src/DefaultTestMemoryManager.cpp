#include "DefaultTestMemoryManager.h"
#include "RCLibTests.h"

#include <chrono>
#include <thread>

namespace RCLib::Tests::Impl
{

DefaultTestMemoryManager::DefaultTestMemoryManager()
  : DefaultTest("TestMemoryManager")
{
}

void DefaultTestMemoryManager::Setup()
{
	// Enable memory tracking before running tests
	IEngine::Get().GetMemoryManager()->EnableTracking(true);
}

void DefaultTestMemoryManager::Run()
{
	TestAllocation();
	TestDeallocation();
	TestTracking();
	TestLeakDetection();
	TestThreadSafety();
}

void DefaultTestMemoryManager::Cleanup() {}

void DefaultTestMemoryManager::TestAllocation()
{
	void* ptr = malloc(100);
	IEngine::Get().GetMemoryManager()->TrackAllocation(ptr, 100, __FILE__, __LINE__);

	if (IEngine::Get().GetMemoryManager()->GetCurrentAllocatedBytes() != 100)
	{
		throw std::runtime_error("Allocation tracking failed");
	}

	free(ptr);
	IEngine::Get().GetMemoryManager()->TrackDeallocation(ptr);
}

void DefaultTestMemoryManager::TestDeallocation()
{
	void* ptr = malloc(200);
	IEngine::Get().GetMemoryManager()->TrackAllocation(ptr, 200, __FILE__, __LINE__);

	if (IEngine::Get().GetMemoryManager()->GetCurrentAllocatedBytes() != 200)
	{
		throw std::runtime_error("Deallocation tracking failed");
	}

	free(ptr);
	IEngine::Get().GetMemoryManager()->TrackDeallocation(ptr);

	if (IEngine::Get().GetMemoryManager()->GetCurrentAllocatedBytes() != 0)
	{
		throw std::runtime_error("Deallocation tracking failed");
	}
}

void DefaultTestMemoryManager::TestTracking()
{
	IEngine::Get().GetMemoryManager()->EnableTracking(false);
	void* ptr = malloc(300);
	IEngine::Get().GetMemoryManager()->TrackAllocation(ptr, 300, __FILE__, __LINE__);

	if (IEngine::Get().GetMemoryManager()->GetCurrentAllocatedBytes() != 0)
	{
		throw std::runtime_error("Tracking disable failed");
	}

	free(ptr);
	IEngine::Get().GetMemoryManager()->TrackDeallocation(ptr);
}

void DefaultTestMemoryManager::TestLeakDetection()
{
	void* ptr = malloc(400);
	IEngine::Get().GetMemoryManager()->TrackAllocation(ptr, 400, __FILE__, __LINE__);

	if (!IEngine::Get().GetMemoryManager()->CheckForLeaks())
	{
		throw std::runtime_error("Leak detection failed");
	}

	free(ptr);
	IEngine::Get().GetMemoryManager()->TrackDeallocation(ptr);

	if (IEngine::Get().GetMemoryManager()->CheckForLeaks())
	{
		throw std::runtime_error("Leak detection failed");
	}
}

void DefaultTestMemoryManager::TestThreadSafety()
{
	const int                numThreads     = 4;
	const int                numAllocations = 1000;
	std::vector<std::thread> threads;

	for (int i = 0; i < numThreads; ++i)
	{
		threads.emplace_back([this, numAllocations]() {
			for (int j = 0; j < numAllocations; ++j)
			{
				void* ptr = malloc(100);
				IEngine::Get().GetMemoryManager()->TrackAllocation(ptr, 100, __FILE__, __LINE__);
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				IEngine::Get().GetMemoryManager()->TrackDeallocation(ptr);
				free(ptr);
			}
		});
	}

	for (auto& thread : threads)
	{
		thread.join();
	}

	if (IEngine::Get().GetMemoryManager()->GetCurrentAllocatedBytes() != 0)
	{
		throw std::runtime_error("Thread safety test failed");
	}
}

} // namespace RCLib::Tests::Impl