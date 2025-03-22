#pragma once
#include "RCLibTests_Fwd.h"
#include "ITestEntityManager.h"
#include "DefaultTest.h"
#include <atomic>
#include <mutex>
#include <shared_mutex>
#include <vector>

namespace RCLib::Tests::Impl
{
class RCLIBTESTS_API DefaultTestEntityManager
    : public Tests::Impl::DefaultTest
    , public Tests::ITestEntityManager
{
public:
	DefaultTestEntityManager();
	~DefaultTestEntityManager() override;

	// ITest interface
	void             Setup() override;
	void             Run() override;
	void             Cleanup() override;
	std::string_view GetName() const override;
	std::string_view GetDescription() const override;

protected:
	// ITestEntityManager interface
	void       CreateEntities() override;
	void       DestroyEntities() override;
	void       Stop() override;
	RCLib::IEntityPtr GetRandomEntity(bool remove) override;

private:
	static constexpr int s_numOfWorkingThreadsByAction = 100;
	static constexpr int s_numEntitiesToAddByThread = 100;
	static constexpr int s_numEntitiesToAdd = s_numOfWorkingThreadsByAction * s_numEntitiesToAddByThread;

	std::atomic<int> m_runningTasks{0};
	std::atomic<int> m_entitiesAdded{0};
	std::atomic<int> m_entitiesRemoved{0};

	std::vector<RCLib::IEntityPtr> m_entities;
	std::shared_mutex m_entitiesMutex;
	std::atomic<bool> m_stop{false};
};
} // namespace RCLib::Tests::Impl