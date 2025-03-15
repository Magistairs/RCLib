#pragma once
#include "Serializable.h"

#include <memory>
#include <shared_mutex>
#include <vector>
#include <guiddef.h>

namespace RCLib
{
	class GameObject;
	using GameObjectPtr = std::shared_ptr<GameObject>;

	class RCLIB_API GameObject : public Serializable
	{
	public:
		struct GUIDCompare
		{
			GUIDCompare(REFGUID guid)
				: m_guid(guid)
			{
			}

			bool operator()(const GameObjectPtr& object) const { return object->GetGUID() == m_guid; }

			REFGUID m_guid;
		};

		GameObject();
		void Release();

		void Update();

		GUID GetGUID() const { return m_GUID; }

		void Serialize(bool load, ObjectPtr pJsonObject) override;

	protected:
		void Load() override;

	private:
		GUID                          m_GUID{ 0 };
	};
} // namespace RCLib