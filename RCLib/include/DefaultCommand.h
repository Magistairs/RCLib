#pragma once
#include "ICommand.h"
#include "RCLib_Fwd.h"
#include "Serializable.h"

#include <functional>
#include <string>

namespace RCLib::Impl
{
class RCLIB_API DefaultCommand
  : public ICommand
  , public Serializable
  , public std::enable_shared_from_this<DefaultCommand>
{
public:
	DefaultCommand() = default;
	explicit DefaultCommand(std::string_view name);
	~DefaultCommand() override;

	// ICommand interface implementation
	void Execute() override;
	void Undo() override;
	void ExecuteLater() override;

	// Additional functionality
	void             SetCallback(std::function<void()>&& callback);
	void             SetUndoCallback(std::function<void()>&& callback);
	void             SetShortcut(std::string_view shortcut);
	void             SetDescription(std::string_view description);
	std::string_view GetShortcutString() const;
	std::string_view GetName() const { return m_name; }
	std::string_view GetDescription() const { return m_description; }

protected:
	void Load() override;

private:
	std::string           m_name;
	std::function<void()> m_callback;
	std::function<void()> m_undoCallback;
	std::string           m_shortcutStr;
	std::string           m_description;
};

} // namespace RCLib::Impl