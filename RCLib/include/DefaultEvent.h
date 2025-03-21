#pragma once
#include "IEvent.h"
#include "RCLib_Fwd.h"

#include <string>
#include <string_view>

namespace RCLib::Impl
{

/**
 * @brief Default implementation of the event system
 */
class RCLIB_API DefaultEvent : public IEvent
{
public:
	explicit DefaultEvent(std::string_view name);
	~DefaultEvent() override;

	// IEvent interface
	TEventType GetType() const override;

	// Additional functionality
	std::string_view GetName() const;
	void             SetName(std::string_view name);
	bool             IsValid() const;

private:
	std::string m_name;
	bool        m_isValid{true};
};

} // namespace RCLib::Impl