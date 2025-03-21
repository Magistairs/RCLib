#pragma once
#include "IEntity.h"
#include "RCLib_Fwd.h"

#include <string>
#include <string_view>

namespace RCLib
{
namespace Impl
{
/**
 * @brief Default implementation of the entity system
 */
class RCLIB_API DefaultEntity : public IEntity
{
public:
	DefaultEntity() = default;
	~DefaultEntity() override;

	// IEntity interface
	std::string_view GetName() const override;
	void             SetName(std::string_view name) override;
	bool             IsValid() const override;
	TEntityId        GetId() const override { return m_guid; }

	virtual void Load();

protected:
	TEntityId   m_guid{0};
	std::string m_name;
	bool        m_isValid{true};
};

} // namespace Impl
} // namespace RCLib