#pragma once
#include "RCLibTests_Fwd.h"
#include "DLLExport.h"
#include "ITestEngine.h"
#include "DefaultTest.h"
#include "DefaultEngine.h"

namespace RCLib::Tests::Impl
{
/**
 * @brief Implementation of the Engine test
 * @ingroup Tests
 */
class RCLIB_API TestEngineImpl
	: public RCLib::Tests::DefaultTest
	, public RCLib::Tests::ITestEngine
{
public:
	// ITest interface implementation
	void Setup() override;
	void Run() override;
	void Cleanup() override;

protected:
	// Test methods that can be overridden by derived classes
	void TestManagerLifecycle() override;
	void TestCustomManagerRegistration() override;
	void TestManagerUpdateOrder() override;
	void TestManagerExceptions() override;

private:
	// Private utility methods specific to this implementation
	void ValidateManagerCount(size_t expectedCount);
};
} // namespace RCLib::Tests::Impl