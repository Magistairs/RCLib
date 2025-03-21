#include "DefaultTaskManager.h"
#include "RCLib.h"

namespace RCLib::Impl
{

DefaultTaskManager::DefaultTaskManager()
  : m_shouldStop(false)
{
	try
	{
		// Add basic commands
		ICommandManagerPtr commandManager = IEngine::Get().GetCommandManager();

		ICommandPtr showQueueSizeCmd = commandManager->AddCommand("task_show_queue_size");
		showQueueSizeCmd->SetDescription("Shows the current size of the task queue");
		showQueueSizeCmd->SetCallback([this]() { IEngine::Get().GetLogger()->Info("Current task queue size: " + std::to_string(GetTaskCount())); });

		ICommandPtr clearQueueCmd = commandManager->AddCommand("task_clear_queue");
		clearQueueCmd->SetDescription("Clears all pending tasks from the queue");
		clearQueueCmd->SetCallback([this]() {
			std::lock_guard<std::mutex> lock(m_queueMutex);
			m_tasks.clear();
			IEngine::Get().GetLogger()->Info("Task queue cleared");
		});

		ICommandPtr showWorkersCmd = commandManager->AddCommand("task_show_workers");
		showWorkersCmd->SetDescription("Shows the number of active worker threads");
		showWorkersCmd->SetCallback([this]() { IEngine::Get().GetLogger()->Info("Active worker threads: " + std::to_string(m_workerThreads.size())); });

		m_running    = true;
		m_shouldStop = false;

		// Start worker threads
		for (int i = 0; i < NumThreads; ++i)
		{
			try
			{
				m_workerThreads.emplace_back([this] { WorkerThread(); });
			}
			catch (const std::exception& e)
			{
				IEngine::Get().GetLogger()->Error("Failed to create worker thread: " + std::string(e.what()));
			}
		}

		IEngine::Get().GetLogger()->Info("Task manager initialized with " + std::to_string(NumThreads) + " worker threads");
	}
	catch (const std::exception& e)
	{
		IEngine::Get().GetLogger()->Error("Task manager initialization failed: " + std::string(e.what()));
	}
}

DefaultTaskManager::~DefaultTaskManager()
{
	EndAllTasks();
}

bool DefaultTaskManager::OnInitialize()
{
	return true;
}

bool DefaultTaskManager::OnUpdate()
{
	try
	{
		std::vector<FTask> tasksToRun;
		{
			std::lock_guard<std::mutex> lock(m_queueMutex);
			tasksToRun = std::move(m_tasks);
			m_tasks.clear();
		}

		if (!tasksToRun.empty())
		{
			for (auto& task : tasksToRun)
			{
				try
				{
					task();
				}
				catch (const std::exception& e)
				{
					IEngine::Get().GetLogger()->Error("Task execution failed: " + std::string(e.what()));
				}
			}
		}
		return true;
	}
	catch (const std::exception& e)
	{
		IEngine::Get().GetLogger()->Error("Task manager update failed: " + std::string(e.what()));
		return false;
	}
}

void DefaultTaskManager::OnRelease()
{
	EndAllTasks();
	std::lock_guard<std::mutex> lock(m_queueMutex);
	m_tasks.clear();
	m_taskQueue = std::queue<FTask>();
}

void DefaultTaskManager::AddTask(std::function<void()> task)
{
	std::lock_guard<std::mutex> lock(m_queueMutex);
	m_tasks.push_back(std::move(task));
	IEngine::Get().GetLogger()->Debug("New task added to queue");
}

void DefaultTaskManager::EndAllTasks()
{
	IEngine::Get().GetLogger()->Info("Ending all tasks...");
	{
		std::lock_guard<std::mutex> lock(m_queueMutex);
		m_shouldStop = true;
		m_running    = false;
	}
	m_condition.notify_all();

	for (auto& thread : m_workerThreads)
	{
		if (thread.joinable())
		{
			thread.join();
		}
	}
	m_workerThreads.clear();
	IEngine::Get().GetLogger()->Info("All tasks ended and workers stopped");
}

void DefaultTaskManager::WorkerThread()
{
	while (true)
	{
		FTask task;
		{
			std::unique_lock<std::mutex> lock(m_queueMutex);
			m_condition.wait(lock, [this] { return m_shouldStop || !m_taskQueue.empty(); });

			if (m_shouldStop && m_taskQueue.empty())
			{
				IEngine::Get().GetLogger()->Debug("Worker thread stopping - no more tasks");
				return;
			}

			if (!m_taskQueue.empty())
			{
				task = std::move(m_taskQueue.front());
				m_taskQueue.pop();
				IEngine::Get().GetLogger()->Debug("Worker thread picked up new task");
			}
		}

		try
		{
			if (task)
				task();
		}
		catch (const std::exception& e)
		{
			IEngine::Get().GetLogger()->Error("Task execution failed: " + std::string(e.what()));
		}
	}
}

size_t DefaultTaskManager::GetTaskCount() const
{
	std::lock_guard<std::mutex> lock(m_queueMutex);
	return m_tasks.size() + m_taskQueue.size();
}

bool DefaultTaskManager::HasTasks() const
{
	std::lock_guard<std::mutex> lock(m_queueMutex);
	return !m_tasks.empty() || !m_taskQueue.empty();
}

} // namespace RCLib::Impl
