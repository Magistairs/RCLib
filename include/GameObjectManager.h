#pragma once
#include "StdIncludes.h"
#include "DLLExport.h"
#include "GameObject.h"

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
		void          RemoveObject(uint64_t objectGuid);
		GameObjectPtr GetObject(uint64_t objectGuid) const;

		void ForEachObject(const std::function<void(GameObjectPtr)>& func) const;

	protected:
		bool                          m_infoLogEnabled = false;
		mutable std::shared_mutex     m_objectsMutex;
		std::vector<GameObjectPtr>    m_objects;
	};
} // namespace RCLib