#include "DefaultEntityManager.h"
#include "RCLib.h"

#include <algorithm>
#include <iterator>

namespace RCLib::Impl
{

DefaultEntityManager::DefaultEntityManager() {}

bool DefaultEntityManager::OnInitialize()
{
	return true;
}

bool DefaultEntityManager::OnUpdate()
{
	return true;
}

void DefaultEntityManager::OnRelease()
{
	std::lock_guard<std::mutex> lock(m_entityMutex);
	m_entities.clear();
	m_nextEntityId = 1;
}

IEntityPtr DefaultEntityManager::CreateEntity()
{
	std::lock_guard<std::mutex> lock(m_entityMutex);
	TEntityId                   entityId = m_nextEntityId++;
	m_entities[entityId]                 = IFactory::Get()->Create<IEntity>();
	return m_entities[entityId];
}

void DefaultEntityManager::DestroyEntity(IEntityPtr entity)
{
	if (!entity)
		return;

	std::lock_guard<std::mutex> lock(m_entityMutex);
	m_entities.erase(entity->GetId());
}

bool DefaultEntityManager::IsValid(TEntityId entityId) const
{
	std::lock_guard<std::mutex> lock(m_entityMutex);
	return m_entities.find(entityId) != m_entities.end();
}

IEntityPtr DefaultEntityManager::GetEntity(TEntityId entityId) const
{
	std::lock_guard<std::mutex> lock(m_entityMutex);
	auto                        it = m_entities.find(entityId);
	if (it != m_entities.end())
	{
		return it->second;
	}
	return nullptr;
}

std::vector<IEntityPtr> DefaultEntityManager::GetAllEntities() const
{
	std::lock_guard<std::mutex> lock(m_entityMutex);
	std::vector<IEntityPtr>     result;
	result.reserve(m_entities.size());
	std::transform(m_entities.begin(), m_entities.end(), std::back_inserter(result), [](const auto& pair) { return pair.second; });
	return result;
}

void DefaultEntityManager::ForEachEntity(const std::function<void(IEntityPtr)>& func) const
{
	std::vector<IEntityPtr> entities;
	{
		std::lock_guard<std::mutex> lock(m_entityMutex);
		entities.reserve(m_entities.size());
		for (const auto& [_, entity] : m_entities)
		{
			entities.push_back(entity);
		}
	}

	for (const auto& entity : entities)
	{
		func(entity);
	}
}

} // namespace RCLib::Impl
