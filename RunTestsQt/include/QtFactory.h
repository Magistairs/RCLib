#pragma once
#include "QtLogger.h"

#include <DefaultFactory.h>
#include <RCLib.h>

namespace RCLibQt
{

/**
 * @brief Factory for creating Qt-specific implementations of RCLib components
 */
class RCLIB_API QtFactory : public RCLib::Impl::DefaultFactory
{
public:
	QtFactory()                    = default;
	~QtFactory() noexcept override = default;

protected:
	RCLib::SharedPtr<void> DoCreate(const std::type_info& type, const CreateArgs& args = {}) override;
};

} // namespace RCLibQt