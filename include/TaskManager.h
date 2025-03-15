#pragma once
#include "StdIncludes.h"
#include "DLLExport.h"

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

namespace RCLib
{
	class RCLIB_API TaskManager
	{
	public:
		static const int NumThreads = 50;

		static TaskManager& Get();

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
		std::condition_variable           m_condition;

		std::mutex        m_queueMutex;
		std::atomic<bool> m_stop{ false };
	};
} // namespace RCLib