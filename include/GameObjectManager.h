#pragma once
#include "GameObject.h"

#include <guiddef.h>
#include <memory>
#include <shared_mutex>
#include <vector>

namespace RCLib
{
	class RCLIB_API GameObjectManager
	{
	public:
		static GameObjectManager& Get();

		GameObjectManager();
		~GameObjectManager();

		void Init();

		GameObjectPtr CreateObject();
		void          RemoveObject(REFGUID objectGUID);
		GameObjectPtr GetObject(REFGUID objectGUID) const;

		void ForEachObject(const std::function<void(GameObjectPtr)>& func) const;

	protected:
		bool                          m_infoLogEnabled = false;
		mutable std::shared_mutex     m_objectsMutex;
		std::vector<GameObjectPtr>    m_objects;
	};
} // namespace RCLib