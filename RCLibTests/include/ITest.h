#pragma once
#include "RCLibTests_Fwd.h"

#include <string_view>

namespace RCLib::Tests
{
/**
 * @brief Interface for test classes
 *
 * Base interface that all test classes must implement.
 * Provides methods for test lifecycle management.
 */
class RCLIB_TESTS_API ITest
{
public:
	virtual ~ITest() = default;

	/**
	 * @brief Setup the test environment
	 */
	virtual void Setup() = 0;

	/**
	 * @brief Run the test
	 */
	virtual void Run() = 0;

	/**
	 * @brief Cleanup after test execution
	 */
	virtual void Cleanup() = 0;

	/**
	 * @brief Get the name of the test
	 * @return Test name
	 */
	virtual std::string_view GetName() const = 0;

	/**
	 * @brief Get the description of the test
	 * @return Test description
	 */
	virtual std::string_view GetDescription() const = 0;

protected:
	ITest() = default;
};

} // namespace RCLib::Tests