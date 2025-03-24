#pragma once
#include "ITest.h"
#include "RCLibTests_Fwd.h"

#include <stdexcept>
#include <functional>
#include <string>
#include <string_view>
#include <vector>

namespace RCLib::Tests::Impl
{

/**
 * @brief Base class for test implementations providing common testing functionality
 * @ingroup Tests
 */
class RCLIB_TESTS_API DefaultTest : public ITest
{
public:
	/**
	 * @brief Creates a test with the specified name and description
	 * @param name Name of the test
	 * @param description Optional description of the test's purpose
	 */
	DefaultTest(std::string_view name, std::string_view description = "");
	~DefaultTest() override = default;

	/**
	 * @brief Gets the test name
	 * @return Name of the test
	 */
	std::string_view GetName() const override { return m_name; }

	/**
	 * @brief Gets the test description
	 * @return Description of the test's purpose
	 */
	std::string_view GetDescription() const override { return m_description; }

protected:
	/**
	 * @brief Executes a test case with error handling and logging
	 * @param name Name of the test case
	 * @param testFunc Function containing the test logic
	 */
	void RunTest(std::string_view name, std::function<void()> testFunc);

	/**
	 * @brief Logs an informational message to the test output
	 * @param message Message to log
	 */
	void LogInfo(std::string_view message);

	/**
	 * @brief Asserts that a condition is true
	 * @param condition Condition to verify
	 * @param message Message to display if assertion fails
	 * @throws std::runtime_error if condition is false
	 */
	void AssertTrue(bool condition, std::string_view message);

	/**
	 * @brief Asserts that a condition is false
	 * @param condition Condition to verify
	 * @param message Message to display if assertion fails
	 * @throws std::runtime_error if condition is true
	 */
	void AssertFalse(bool condition, std::string_view message);

	/**
	 * @brief Asserts that two strings are equal
	 * @param expected Expected string value
	 * @param actual Actual string value
	 * @param message Message to display if assertion fails
	 * @throws std::runtime_error if strings are not equal
	 */
	void AssertStringEqual(std::string_view expected, std::string_view actual, std::string_view message);

	/**
	 * @brief Asserts that two values are equal
	 * @tparam T Type of values to compare
	 * @param expected Expected value
	 * @param actual Actual value
	 * @param message Message to display if assertion fails
	 * @throws std::runtime_error if values are not equal
	 */
	template<typename T>
	void AssertEqual(const T& expected, const T& actual, std::string_view message);

	/**
	 * @brief Asserts that a pointer is not null
	 * @tparam T Type of pointer to check
	 * @param ptr Pointer to verify
	 * @param message Message to display if assertion fails
	 * @throws std::runtime_error if pointer is null
	 */
	template<typename T>
	void AssertNotNull(const T* ptr, std::string_view message);

private:
	std::string m_name;
	std::string m_description;
};

template<typename T>
void DefaultTest::AssertEqual(const T& expected, const T& actual, std::string_view message)
{
	if (expected != actual)
	{
		throw std::runtime_error(std::string(message));
	}
}

template<typename T>
void DefaultTest::AssertNotNull(const T* ptr, std::string_view message)
{
	if (ptr == nullptr)
	{
		throw std::runtime_error(std::string(message));
	}
}

} // namespace RCLib::Tests::Impl