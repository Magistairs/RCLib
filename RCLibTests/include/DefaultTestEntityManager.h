#pragma once
#include "DefaultTest.h"
#include "ITestEntityManager.h"
#include "RCLibTests_Fwd.h"

#include <atomic>
#include <mutex>
#include <shared_mutex>
#include <vector>

namespace RCLib::Tests::Impl
{
/**
 * @brief Implementation of the EntityManager test
 * @ingroup Tests
 */
class RCLIB_TESTS_API DefaultTestEntityManager : public DefaultTest, public ITestEntityManager
{
public:
	DefaultTestEntityManager();
	~DefaultTestEntityManager() override;

	// ITest interface
	void             Setup() override;
	void             Run() override;
	void             Cleanup() override ;
	std::string_view GetName() const override { return "DefaultTestEntityManager"; }
	std::string_view GetDescription() const override { return "Tests the EntityManager's functionality and thread safety"; }

protected:
	// ITestEntityManager interface
	void              CreateEntities() override;
	void              DestroyEntities() override;
	void              Stop() override;
	RCLib::IEntityPtr GetRandomEntity(bool remove) override;

private:
	static constexpr int s_numOfWorkingThreadsByAction = 100;
	static constexpr int s_numEntitiesToAddByThread    = 100;
	static constexpr int s_numEntitiesToAdd            = s_numOfWorkingThreadsByAction * s_numEntitiesToAddByThread;

	std::atomic<int> m_runningTasks{0};
	std::atomic<int> m_entitiesAdded{0};
	std::atomic<int> m_entitiesRemoved{0};

	std::vector<RCLib::IEntityPtr> m_entities;
	std::shared_mutex              m_entitiesMutex;
	std::atomic<bool>              m_stop{false};
};
} // namespace RCLib::Tests::Impl