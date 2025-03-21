#include "QtFactory.h"

namespace TestsQt
{

RCLib::SharedPtr<void> QtFactory::DoCreate(const std::type_info& type, const CreateArgs& args)
{
	if (type == typeid(RCLib::ILogger))
	{
		return RCLib::MakeShared<QtLogger>();
	}

	// For other types, use the default implementation
	return RCLib::Impl::DefaultFactory::CreateImpl(type, args);
}

} // namespace TestsQt