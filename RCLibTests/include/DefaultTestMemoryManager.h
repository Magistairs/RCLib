#pragma once
#include "RCLibTests_Fwd.h"
#include "DLLExport.h"
#include "ITestMemoryManager.h"
#include "DefaultTest.h"
#include "SmartPointers.h"

namespace RCLib::Tests::Impl
{

/**
 * @brief Implementation of memory manager tests
 */
class RCLIB_API TestMemoryManagerImpl
  : public RCLib::Tests::ITestMemoryManager
  , public DefaultTest
{
public:
	TestMemoryManagerImpl();
	~TestMemoryManagerImpl() override = default;

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

private:
	SharedPtr<MemoryManagerImpl> m_manager;
};

} // namespace RCLib::Tests::Impl