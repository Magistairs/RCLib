#include "TestMain.h"

#include <RCLib.h>

#include <iostream>

namespace Tests
{

TestMain::TestMain()
{
}

bool TestMain::OnInitialize()
{
   return true;
}

bool TestMain::OnUpdate()
{
    return true;
}

void TestMain::OnRelease()
{
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