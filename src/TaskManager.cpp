#include "TaskManager.h"

using namespace RCLib;

TaskManager::TaskManager()
	: m_stop(false)
{
	for (int i = 0; i < NumThreads; ++i)
	{
		m_workers.emplace_back([this] {
			while (true)
			{
				std::function<void()> task;

				{
					std::unique_lock<std::mutex> lock(m_queueMutex);

					// Wait for a task if the queue is empty and the stop flag is not set
					m_condition.wait(lock, [this] { return m_stop || !m_taskQueue.empty(); });

					if (m_stop)
						return;

					// Get the task from the front of the queue
					task = std::move(m_taskQueue.front());
					m_taskQueue.pop();
				}

				// Execute the task
				task();
			}
			});
	}
}

TaskManager::~TaskManager() {}

void TaskManager::EndAllTasks()
{
	{
		std::unique_lock<std::mutex> lock(m_queueMutex);
		m_stop = true;
	}

	// Notify all threads to wake up and exit
	m_condition.notify_all();

	// Join all threads
	for (std::thread& worker : m_workers)
	{
		worker.join();
	}
}
