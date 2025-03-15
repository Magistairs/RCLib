#pragma once
#include "StdIncludes.h"
#include "Object.h"
#include "Serializable.h"

namespace RCLib
{
	class GameObject;
	using GameObjectPtr = std::shared_ptr<GameObject>;

	class RCLIB_API GameObject : public Serializable
	{
	public:
		struct GUIDCompare
		{
			GUIDCompare(uint64_t guid)
				: m_guid(guid)
			{
			}

			bool operator()(const GameObjectPtr& object) const { return object->GetGUID() == m_guid; }

			uint64_t m_guid;
		};

		GameObject();
		void Release();

		void Update();

		uint64_t GetGUID() const { return m_guid; }

	protected:
		void Load() override;

	private:
		uint64_t m_guid{ 0 };
	};
} // namespace RCLib