#pragma once
#include "DLLExport.h"
#include "EventSystem.h"
#include "Serializable.h"

#include <functional>
#include <memory>
#include <string>

namespace RCLib
{
	class RCLIB_API Action : public Serializable
	{
	public:
		explicit Action(const char* name);

		void Execute();
		void ExecuteLater();

		void               SetCallback(std::function<void()>&& callback);
		void               SetShortcut(const std::string& shortcut);
		std::string        GetShortcutString() const;
		const std::string& GetName() const { return m_name; }

		void Serialize(bool load, ObjectPtr pJsonObject) override;

		static void SetActionsToExecute(std::vector<std::string>& actionsToExecute);

	protected:
		void Load() override;

	private:
		std::string           m_name;
		std::function<void()> m_callback;
		//sf::Event::KeyEvent   m_shortcut;
		std::string           m_shortcutStr;
		std::string           m_description;
		EventSystem           m_eventSystem;

		inline static bool                     s_logEnabled{ false };
		inline static std::vector<std::string> s_actionsToExecute;
	};
	using ActionPtr = std::shared_ptr<Action>;
} // namespace RCLib