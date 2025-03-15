#pragma once
#include "StdIncludes.h"
#include "DLLExport.h"
#include "Action.h"
#include "Serializable.h"
#include "SmartPtr.h"
#include "Object.h"

#include <functional>
#include <vector>

namespace RCLib
{
	class RCLIB_API ActionManager : public Serializable
	{
	public:
		static ActionManager& Get();

		ActionManager();
		~ActionManager();

		ActionPtr AddAction(const char* name);
		bool      ExecuteAction(const char* name);

	protected:
		void Save() override;
		void Load() override;

	private:
		std::vector<ActionPtr> m_actions;
	};
} // namespace RCLib