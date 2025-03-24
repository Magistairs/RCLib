#pragma once
#include "QtLogger.h"

#include <DefaultFactory.h>
#include <RCLib.h>

#include <memory>
#include <typeinfo>

namespace TestsQt
{

/**
 * @brief Factory for creating Qt-specific implementations of RCLib components
 */
class QtFactory : public RCLib::Impl::DefaultFactory
{
public:
	QtFactory()                    = default;
	~QtFactory() noexcept override = default;

protected:
	SharedPtr<void> CreateImpl(const std::type_info& type) override;
};

} // namespace TestsQt