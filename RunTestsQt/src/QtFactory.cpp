#include "QtFactory.h"

namespace TestsQt
{

SharedPtr<void> QtFactory::CreateImpl(const std::type_info& type)
{
	if (type == typeid(RCLib::ILogger))
	{
		return MakeShared<QtLogger>();
	}

	// For other types, use the default implementation
	return RCLib::Impl::DefaultFactory::CreateImpl(type);
}

} // namespace TestsQt