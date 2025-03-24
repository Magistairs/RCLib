#pragma once

#include "ITestRunner.h"
#include "RCLibTests_Fwd.h"

#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <string_view>
#include <vector>

namespace RCLib::Tests::Impl
{

/**
 * @brief Default implementation of the test runner
 * @ingroup Tests
 */
class RCLIB_TESTS_API DefaultTestRunner final : public ITestRunner
{
public:
	DefaultTestRunner();
	~DefaultTestRunner() override;

	// ITestRunner interface implementation
	bool   RunAllTests() override;
	size_t GetTestCount() const override;
	size_t GetPassedCount() const override;
	size_t GetFailedCount() const override;

	// Test management methods
	void AddTest(std::string_view name, std::function<bool()> testFunc);

private:
	struct Test
	{
		std::string           name;
		std::function<bool()> func;
		bool                  passed{false};
		bool                  executed{false};
	};

	// Mutex for thread safety
	mutable std::mutex        m_mutex;
	std::vector<Test> m_tests;
	size_t            m_passedCount{0};
	size_t            m_failedCount{0};
};

} // namespace RCLib::Tests::Impl