#include <StdIncludes.h>

class TaskManager
{
public:
	static const int NumThreads = 32;

	TaskManager();
	~TaskManager();

	void EndAllTasks();

	template<typename Function>
	void AddTask(Function&& func)
	{
		std::function<void()> task = std::bind(std::forward<Function>(func));

		{
			std::unique_lock<std::mutex> lock(m_queueMutex);
			m_taskQueue.emplace(std::move(task));
		}

		// Notify one waiting thread to wake up and process the task
		m_condition.notify_one();
	}

private:
	std::vector<std::thread>          m_workers;
	std::queue<std::function<void()>> m_taskQueue;
	std::mutex                        m_queueMutex;
	std::condition_variable           m_condition;

	bool m_stop;
};
