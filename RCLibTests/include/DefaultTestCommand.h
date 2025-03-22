#pragma once

#include "ICommand.h"
#include "RCLibTests_Fwd.h"

#include <string>
#include <string_view>
#include <functional>

namespace RCLib::Tests::Impl
{

/**
 * @brief Default implementation of a test command
 * @ingroup Tests
 */
class RCLIBTESTS_API DefaultTestCommand : public RCLib::ICommand
{
public:
	DefaultTestCommand(std::string_view name, std::function<void()> execute, std::function<void()> undo);
	~DefaultTestCommand() override = default;

	// ICommand interface
	void Execute() override;
	void Undo() override;
	std::string_view GetName() const override { return m_name; }

private:
	std::string m_name;
	std::function<void()> m_execute;
	std::function<void()> m_undo;
};

} // namespace RCLib::Tests::Impl 