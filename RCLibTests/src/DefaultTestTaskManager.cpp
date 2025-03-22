#include "DefaultTestTaskManager.h"
#include "RCLibTests.h"

namespace RCLib::Impl
{

TestTaskManagerImpl::TestTaskManagerImpl()
  : DefaultTest("TestTaskManager")
{
	m_taskCount      = 0;
	m_completedTasks = 0;
}

void TestTaskManagerImpl::Setup()
{
	m_manager = MakeShared<TaskManagerImpl>();
	m_manager->Initialize();
}

void TestTaskManagerImpl::Run()
{
	TestTaskSubmission();
	TestTaskExecution();
	TestTaskCancellation();
	TestThreadSafety();
}

void TestTaskManagerImpl::Cleanup()
{
	if (m_manager)
	{
		m_manager->Release();
		m_manager.reset();
	}
}

void TestTaskManagerImpl::TestTaskSubmission()
{
	const int numTasks = 100;
	for (int i = 0; i < numTasks; ++i)
	{
		m_manager->AddTask([this]() {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			++m_completedTasks;
		});
		++m_taskCount;
	}

	// Wait for tasks to complete
	std::unique_lock<std::mutex> lock(m_mutex);
	m_cv.wait(lock, [this]() { return m_completedTasks == m_taskCount; });

	AssertEqual(m_taskCount, m_completedTasks, "All tasks should complete");
}

void TestTaskManagerImpl::TestTaskExecution()
{
	bool executed = false;
	m_manager->AddTask([&executed]() { executed = true; });

	// Wait for task to complete
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	AssertTrue(executed, "Task should be executed");
}

void TestTaskManagerImpl::TestTaskCancellation()
{
	const int numTasks = 50;
	for (int i = 0; i < numTasks; ++i)
	{
		m_manager->AddTask([this]() {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			++m_completedTasks;
		});
		++m_taskCount;
	}

	// Cancel all tasks
	m_manager->EndAllTasks();

	// Wait for remaining tasks to complete
	std::unique_lock<std::mutex> lock(m_mutex);
	m_cv.wait(lock, [this]() { return m_completedTasks == m_taskCount; });

	AssertEqual(m_taskCount, m_completedTasks, "All tasks should complete after cancellation");
}

void TestTaskManagerImpl::TestThreadSafety()
{
	const int                numThreads     = 10;
	const int                tasksPerThread = 100;
	std::vector<std::thread> threads;

	for (int i = 0; i < numThreads; ++i)
	{
		threads.emplace_back([this, tasksPerThread]() {
			for (int j = 0; j < tasksPerThread; ++j)
			{
				m_manager->AddTask([this]() {
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
					++m_completedTasks;
				});
				++m_taskCount;
			}
		});
	}

	for (auto& thread : threads)
	{
		thread.join();
	}

	// Wait for all tasks to complete
	std::unique_lock<std::mutex> lock(m_mutex);
	m_cv.wait(lock, [this]() { return m_completedTasks == m_taskCount; });

	AssertEqual(m_taskCount, m_completedTasks, "All tasks should complete in thread-safe manner");
}

} // namespace RCLib::Impl