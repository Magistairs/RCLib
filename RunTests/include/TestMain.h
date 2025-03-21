#pragma once
#include <RCLib_Fwd.h>
#include <DefaultStateWrapper.h>

namespace Tests
{
/**
 * @brief Main test execution class
 */
class TestMain : public RCLib::IState
{
public:
    TestMain();
    ~TestMain() override = default;

    // ITriState interface
    bool OnInitialize() override;
    bool OnUpdate() override;
    void OnRelease() override;

    /**
     * @brief Run all tests
     * @return true if all tests passed, false otherwise
     */
    bool RunTests();

private:
    RCLib::Impl::DefaultStateWrapper<RCLib::IEngine> m_engine;
};
} // namespace Tests 