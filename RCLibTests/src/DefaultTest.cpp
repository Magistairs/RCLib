#include "DefaultTest.h"
#include "RCLibTests.h"

#include <stdexcept>

namespace RCLib::Tests::Impl
{

DefaultTest::DefaultTest(std::string_view name, std::string_view description)
  : m_name(name)
  , m_description(description)
{
}

void DefaultTest::RunTest(std::string_view name, std::function<void()> testFunc)
{
	try
	{
		LogInfo("Running test: " + std::string(name));
		testFunc();
		LogInfo("Test passed: " + std::string(name));
	}
	catch (const std::exception& e)
	{
		LogInfo("Test failed: " + std::string(name) + " - " + e.what());
		throw;
	}
}

void DefaultTest::LogInfo(std::string_view message)
{
	auto& engine = IEngine::Get();
	if (auto logger = engine.GetLogger())
	{
		logger->Info(message);
	}
}

void DefaultTest::AssertTrue(bool condition, std::string_view message)
{
	if (!condition)
	{
		throw std::runtime_error(std::string(message));
	}
}

void DefaultTest::AssertFalse(bool condition, std::string_view message)
{
	AssertTrue(!condition, message);
}

void DefaultTest::AssertStringEqual(std::string_view expected, std::string_view actual, std::string_view message)
{
	if (expected != actual)
	{
		throw std::runtime_error(std::string(message) + " - Expected: " + std::string(expected) + ", Actual: " + std::string(actual));
	}
}

// Explicit template instantiations for common types
template void DefaultTest::AssertEqual<int>(const int&, const int&, std::string_view);
template void DefaultTest::AssertEqual<size_t>(const size_t&, const size_t&, std::string_view);
template void DefaultTest::AssertEqual<bool>(const bool&, const bool&, std::string_view);
template void DefaultTest::AssertEqual<float>(const float&, const float&, std::string_view);
template void DefaultTest::AssertEqual<double>(const double&, const double&, std::string_view);

} // namespace RCLib::Tests::Impl