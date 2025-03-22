#include "DefaultTestCommand.h"
#include "RCLibTests.h"
#include "IEngine.h"
#include <stdexcept>
#include <string>
#include <string_view>
#include <functional>

namespace RCLib::Impl
{

DefaultTestCommand::DefaultTestCommand(std::string_view name, std::function<void()> execute, std::function<void()> undo)
    : m_name(name)
    , m_execute(execute)
    , m_undo(undo)
{
}

void DefaultTestCommand::Execute()
{
    if (!m_execute)
    {
        throw std::runtime_error("No execute function defined for command: " + m_name);
    }
    m_execute();
}

void DefaultTestCommand::Undo()
{
    if (!m_undo)
    {
        throw std::runtime_error("No undo function defined for command: " + m_name);
    }
    m_undo();
}

} // namespace RCLib::Impl 