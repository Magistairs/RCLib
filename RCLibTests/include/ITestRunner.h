#pragma once
#include "RCLibTests_Fwd.h"

namespace RCLib::Tests
{
/**
 * @brief Interface for running tests and managing test suites
 * @ingroup Core
 */
class RCLIB_TESTS_API ITestRunner
{
public:
	virtual ~ITestRunner() = default;

	/**
	 * @brief Runs all registered tests
	 * @return True if all tests passed, false otherwise
	 */
	virtual bool RunAllTests() = 0;

	/**
	 * @brief Gets the total number of tests
	 * @return Number of registered tests
	 */
	virtual size_t GetTestCount() const = 0;

	/**
	 * @brief Gets the number of passed tests
	 * @return Number of tests that passed
	 */
	virtual size_t GetPassedCount() const = 0;

	/**
	 * @brief Gets the number of failed tests
	 * @return Number of tests that failed
	 */
	virtual size_t GetFailedCount() const = 0;
};

} // namespace RCLib::Tests