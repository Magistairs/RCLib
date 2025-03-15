#pragma once
#include "Action.h"
#include "DLLExport.h"
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

		void Serialize(bool load, ObjectPtr pJsonObject) override;

	protected:
		void Save() override;
		void Load() override;

	private:
		std::vector<ActionPtr> m_actions;
	};
} // namespace RCLib