#pragma once

#include "RCLibTests_Fwd.h"
#include "DLLExport.h"
#include "ITestRunner.h"

#include <mutex>
#include <vector>
#include <functional>
#include <string>
#include <string_view>
#include <memory>

namespace RCLib::Tests::Impl
{

/**
 * @brief Default implementation of the test runner
 * @ingroup Tests
 */
class RCLIBTESTS_API DefaultTestRunner final : public ITestRunner
{
public:
	DefaultTestRunner();
	~DefaultTestRunner() override;

	// ITestRunner interface implementation
	bool RunAllTests() override;
	size_t GetTestCount() const override;
	size_t GetPassedCount() const override;
	size_t GetFailedCount() const override;

	// Test management methods
	void AddTest(std::string_view name, std::function<bool()> testFunc);

private:
	struct Test {
		std::string name;
		std::function<bool()> func;
		bool passed{false};
		bool executed{false};
	};

	// Mutex for thread safety
	std::mutex m_mutex;
	std::vector<Test> m_tests;
	size_t m_passedCount{0};
	size_t m_failedCount{0};
};

} // namespace RCLib::Tests::Impl