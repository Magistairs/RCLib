#include "DefaultEvent.h"
#include "RCLib.h"

namespace RCLib::Impl
{

DefaultEvent::DefaultEvent(std::string_view name)
  : m_name(name)
{
}

DefaultEvent::~DefaultEvent() = default;

TEventType DefaultEvent::GetType() const
{
	return typeid(*this).hash_code();
}

std::string_view DefaultEvent::GetName() const
{
	return m_name;
}

void DefaultEvent::SetName(std::string_view name)
{
	m_name = name;
}

bool DefaultEvent::IsValid() const
{
	return m_isValid;
}

} // namespace RCLib::Impl