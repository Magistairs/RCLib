#pragma once

#include "ICommandManager.h"
#include "RCLib_Fwd.h"

#include <mutex>
#include <queue>
#include <unordered_map>

namespace RCLib::Impl
{

/**
 * @brief Default implementation of the command management system
 */
class RCLIB_API DefaultCommandManager : public ICommandManager
{
public:
	DefaultCommandManager();
	~DefaultCommandManager() override;

	// ICommandManager interface
	void   QueueCommand(ICommandPtr command) override;
	void   ExecuteCommand(ICommandPtr command) override;
	void   ClearCommandQueue() override;
	size_t GetQueueSize() const override;

	// Additional command management methods
	ICommandPtr AddCommand(std::string_view name) override;
	ICommandPtr              GetCommand(std::string_view name) const override;
	bool        ExecuteCommand(std::string_view name) override;
	void        Save() override;
	void        Load() override;
	bool        Undo() override;

	// IState interface
	bool OnInitialize() override;
	bool OnUpdate() override;
	void OnRelease() override;

	// Additional utility methods
	std::vector<std::string> GetCommandNames() const;

private:
	std::unordered_map<std::string, ICommandPtr> m_commands;
	std::queue<ICommandPtr>                      m_commandQueue;
	std::vector<ICommandPtr>                     m_commandsHistory;
	mutable std::mutex                           m_mutex;
	mutable std::mutex                           m_queueMutex;
	bool                                         m_isLoaded{false};
};
} // namespace RCLib::Impl