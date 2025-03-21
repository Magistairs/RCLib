#include "DefaultEntity.h"
#include "RCLib.h"

namespace RCLib::Impl
{

DefaultEntity::~DefaultEntity() = default;

std::string_view DefaultEntity::GetName() const
{
	return m_name;
}

void DefaultEntity::SetName(std::string_view name)
{
	m_name = name;
}

bool DefaultEntity::IsValid() const
{
	return m_isValid;
}

void DefaultEntity::Load()
{
	// Base implementation does nothing
}

} // namespace RCLib::Impl