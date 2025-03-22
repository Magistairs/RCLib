#include "DefaultTestRunner.h"
#include "RCLibTests.h"
#include <stdexcept>
#include <algorithm>

namespace RCLib::Impl
{

DefaultTestRunner::DefaultTestRunner() = default;

DefaultTestRunner::~DefaultTestRunner() = default;

bool DefaultTestRunner::RunAllTests()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_passedCount = 0;
    m_failedCount = 0;

    for (auto& test : m_tests)
    {
        try
        {
            test.executed = true;
            test.passed = test.func();
            
            if (test.passed)
                m_passedCount++;
            else
                m_failedCount++;
        }
        catch (const std::exception&)
        {
            test.executed = true;
            test.passed = false;
            m_failedCount++;
        }
    }

    return m_failedCount == 0;
}

size_t DefaultTestRunner::GetTestCount() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_tests.size();
}

size_t DefaultTestRunner::GetPassedCount() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_passedCount;
}

size_t DefaultTestRunner::GetFailedCount() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_failedCount;
}

void DefaultTestRunner::AddTest(std::string_view name, std::function<bool()> testFunc)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    
    auto it = std::find_if(m_tests.begin(), m_tests.end(),
        [&name](const Test& test) { return test.name == name; });
        
    if (it != m_tests.end())
    {
        throw std::runtime_error("Test already exists: " + std::string(name));
    }

    m_tests.emplace_back(Test{
        .name = std::string(name),
        .func = std::move(testFunc),
        .passed = false,
        .executed = false
    });
}

} // namespace RCLib::Impl