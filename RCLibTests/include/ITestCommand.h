#pragma once
#include "DLLExport.h"
#include <string_view>

namespace RCLib::Tests
{

class RCLIB_API ITestCommand
{
public:
    virtual ~ITestCommand() = default;

    virtual std::string_view GetName() const = 0;
    virtual void Execute() = 0;
    virtual void Undo() = 0;
};

} // namespace RCLib::Tests 