#pragma once
#include "RCLib_Fwd.h"

#ifdef _WIN32
#ifdef RCLIB_TESTS_EXPORT
#define RCLIB_TESTS_API __declspec(dllexport)
#else
#define RCLIB_TESTS_API __declspec(dllimport)
#endif
#else
#define RCLIB_TESTS_API
#endif

#pragma warning(disable : 4251) // 4251 can't be avoided with STL types

namespace RCLib
{

namespace Tests
{
RCLIB_DECLARE_PTR(ITest);
RCLIB_DECLARE_PTR(ITestEntityManager);
RCLIB_DECLARE_PTR(ITestCommand);
RCLIB_DECLARE_PTR(ITestEntityManager);
RCLIB_DECLARE_PTR(ITestMemoryManager);
RCLIB_DECLARE_PTR(ITestEventManager);
RCLIB_DECLARE_PTR(ITestMessageManager);
RCLIB_DECLARE_PTR(ITestCommandManager);
RCLIB_DECLARE_PTR(ITestCommand);
} // namespace Tests

} // namespace RCLib