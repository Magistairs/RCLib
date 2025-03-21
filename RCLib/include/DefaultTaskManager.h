#pragma once
#include "ITaskManager.h"
#include "RCLib_Fwd.h"

#include <atomic>
#include <condition_variable>
#include <functional>
#include <queue>
#include <thread>

namespace RCLib::Impl
{
/**
 * @brief Default implementation of the task manager with thread pool support
 * @ingroup Core
 */
class RCLIB_API DefaultTaskManager : public ITaskManager
{
public:
	explicit DefaultTaskManager();
	~DefaultTaskManager() override;

	// ITaskManager interface implementation
	void   AddTask(std::function<void()> task) override;
	void   EndAllTasks() override;
	size_t GetTaskCount() const override;
	bool   HasTasks() const override;

	// Managed interface implementation
	bool OnInitialize() override;
	bool OnUpdate() override;
	void OnRelease() override;

protected:
	/**
	 * @brief Worker thread function
	 *
	 * Main loop for worker threads that:
	 * 1. Waits for tasks using condition variable
	 * 2. Executes tasks from the queue
	 * 3. Handles task execution errors
	 * 4. Terminates when stop signal is received
	 */
	void WorkerThread();

private:
	static constexpr int NumThreads = 4;

	// Thread pool
	std::vector<std::thread> m_workerThreads;
	std::queue<FTask>        m_taskQueue;
	std::atomic<bool>        m_running{false};
	std::atomic<bool>        m_shouldStop{false};

	// Synchronization
	mutable std::mutex      m_queueMutex;
	std::condition_variable m_condition;

	// Task storage
	std::vector<FTask> m_tasks;
};

} // namespace RCLib::Impl