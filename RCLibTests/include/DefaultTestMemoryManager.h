#pragma once
#include "DefaultTest.h"
#include "ITestMemoryManager.h"
#include "RCLibTests_Fwd.h"

namespace RCLib::Tests::Impl
{

/**
 * @brief Implementation of memory manager tests
 */
class RCLIB_TESTS_API DefaultTestMemoryManager
  : public DefaultTest
  , public ITestMemoryManager
{
public:
	DefaultTestMemoryManager();
	~DefaultTestMemoryManager() override = default;

	// ITestMemoryManager interface implementation
	void             Setup() override;
	void             Run() override;
	void             Cleanup() override;
	std::string_view GetName() const override { return "TestMemoryManager"; }
	std::string_view GetDescription() const override { return "Tests the MemoryManager's functionality and thread safety"; }

protected:
	// Test cases
	void TestAllocation() override;
	void TestDeallocation() override;
	void TestTracking() override;
	void TestLeakDetection() override;
	void TestThreadSafety() override;
};

} // namespace RCLib::Tests::Impl