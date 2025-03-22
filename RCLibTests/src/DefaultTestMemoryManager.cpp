#include "DefaultTestMemoryManager.h"
#include "RCLibTests.h"
#include "DefaultEngine.h"

namespace RCLib::Impl
{

TestMemoryManagerImpl::TestMemoryManagerImpl()
  : DefaultTest("TestMemoryManager")
{
	m_manager = MakeShared<MemoryManagerImpl>();
	m_manager->OnInitialize();
}

void TestMemoryManagerImpl::Run()
{
	TestAllocation();
	TestDeallocation();
	TestTracking();
	TestLeakDetection();
	TestThreadSafety();
}

void TestMemoryManagerImpl::Cleanup()
{
	m_manager.reset();
}

void TestMemoryManagerImpl::TestAllocation()
{
	void* ptr = malloc(100);
	m_manager->TrackAllocation(ptr, 100, __FILE__, __LINE__);

	if (m_manager->GetCurrentAllocatedBytes() != 100)
	{
		throw std::runtime_error("Allocation tracking failed");
	}

	free(ptr);
	m_manager->TrackDeallocation(ptr);
}

void TestMemoryManagerImpl::TestDeallocation()
{
	void* ptr = malloc(200);
	m_manager->TrackAllocation(ptr, 200, __FILE__, __LINE__);

	if (m_manager->GetCurrentAllocatedBytes() != 200)
	{
		throw std::runtime_error("Deallocation tracking failed");
	}

	free(ptr);
	m_manager->TrackDeallocation(ptr);

	if (m_manager->GetCurrentAllocatedBytes() != 0)
	{
		throw std::runtime_error("Deallocation tracking failed");
	}
}

void TestMemoryManagerImpl::TestTracking()
{
	m_manager->EnableTracking(false);
	void* ptr = malloc(300);
	m_manager->TrackAllocation(ptr, 300, __FILE__, __LINE__);

	if (m_manager->GetCurrentAllocatedBytes() != 0)
	{
		throw std::runtime_error("Tracking disable failed");
	}

	free(ptr);
	m_manager->TrackDeallocation(ptr);
}

void TestMemoryManagerImpl::TestLeakDetection()
{
	void* ptr = malloc(400);
	m_manager->TrackAllocation(ptr, 400, __FILE__, __LINE__);

	if (!m_manager->CheckForLeaks())
	{
		throw std::runtime_error("Leak detection failed");
	}

	free(ptr);
	m_manager->TrackDeallocation(ptr);

	if (m_manager->CheckForLeaks())
	{
		throw std::runtime_error("Leak detection failed");
	}
}

void TestMemoryManagerImpl::TestThreadSafety()
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
				m_manager->TrackAllocation(ptr, 100, __FILE__, __LINE__);
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				m_manager->TrackDeallocation(ptr);
				free(ptr);
			}
		});
	}

	for (auto& thread : threads)
	{
		thread.join();
	}

	if (m_manager->GetCurrentAllocatedBytes() != 0)
	{
		throw std::runtime_error("Thread safety test failed");
	}
}

} // namespace RCLib::Impl