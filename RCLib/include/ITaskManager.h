#pragma once
#include "IState.h"
#include "RCLib_Fwd.h"

namespace RCLib
{

using FTask = std::function<void()>;

/**
 * @brief Interface for the task management system
 *
 * Provides functionality for asynchronous task execution and management.
 * Tasks are executed in a thread pool to maximize performance and resource utilization.
 */
class RCLIB_API ITaskManager : public IState
{
public:
	/**
	 * @brief Virtual destructor
	 */
	virtual ~ITaskManager() = default;

	/**
	 * @brief Adds a task to the execution queue
	 *
	 * The task will be executed asynchronously by one of the worker threads.
	 * If the manager is not initialized, the task will be rejected.
	 *
	 * @param task Function to be executed asynchronously
	 */
	virtual void AddTask(FTask task) = 0;

	/**
	 * @brief Stops all running tasks and cleans up resources
	 *
	 * This method:
	 * 1. Stops accepting new tasks
	 * 2. Waits for running tasks to complete
	 * 3. Cleans up thread pool resources
	 */
	virtual void EndAllTasks() = 0;

	/**
	 * @brief Returns the current number of tasks in the queue
	 * @return Number of pending tasks
	 */
	virtual size_t GetTaskCount() const = 0;

	/**
	 * @brief Checks if there are any tasks in the queue
	 * @return true if there are tasks, false otherwise
	 */
	virtual bool HasTasks() const = 0;

protected:
	ITaskManager() = default;
};

} // namespace RCLib