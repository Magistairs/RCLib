#include "TestMain.h"

#include <RCLib.h>

#include <iostream>

namespace Tests
{

TestMain::TestMain()
    : m_engine("Engine", RCLib::IFactory::Get()->Create<RCLib::IEngine>())
{
}

bool TestMain::OnInitialize()
{
   return  m_engine.Get()->OnInitialize();
}

bool TestMain::OnUpdate()
{
    while(m_engine.Get()->OnUpdate());
    return true;
}

void TestMain::OnRelease()
{
    m_engine.Release();
}

bool TestMain::RunTests()
{
    try
    {
        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return false;
    }
}

} // namespace Tests 