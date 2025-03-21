#include "TestMain.h"
#include "RCLib.h"
#include <iostream>

namespace Tests
{

TestMain::TestMain()
    : m_engine(RCLib::IEngine::Get().GetFactory()->Create<RCLib::IEngine>())
{
}

void TestMain::OnInitialize()
{
    m_engine->Initialize();
}

void TestMain::OnUpdate()
{
    while(m_engine->Update());
}

void TestMain::OnRelease()
{
    m_engine->Release();
}

bool TestMain::RunTests()
{
    try
    {
        m_engine->GetTestRunner()->RunAllTests();
        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return false;
    }
}

} // namespace Tests 