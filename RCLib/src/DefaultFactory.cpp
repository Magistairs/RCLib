#include "DefaultFactory.h"
#include "DefaultCommand.h"
#include "DefaultCommandManager.h"
#include "DefaultEngine.h"
#include "DefaultEntityManager.h"
#include "DefaultEventManager.h"
#include "DefaultLogger.h"
#include "DefaultMemoryManager.h"
#include "DefaultMessageManager.h"
#include "DefaultStateWrapper.h"
#include "DefaultTaskManager.h"
#include "RCLib.h"

namespace RCLib
{

IFactoryPtr RCLib::IFactory::Get()
{
	return MakeShared<Impl::DefaultFactory>();
}
} // namespace RCLib

namespace RCLib::Impl
{

SharedPtr<void> DefaultFactory::CreateImpl(const std::type_info& type)
{
	if (type == typeid(IEngine))
		return std::static_pointer_cast<void>(MakeShared<DefaultEngine>());
	else if (type == typeid(ILogger))
		return std::static_pointer_cast<void>(MakeShared<DefaultLogger>());
	else if (type == typeid(ICommandManager))
		return std::static_pointer_cast<void>(MakeShared<DefaultCommandManager>());
	else if (type == typeid(IEntityManager))
		return std::static_pointer_cast<void>(MakeShared<DefaultEntityManager>());
	else if (type == typeid(IEventManager))
		return std::static_pointer_cast<void>(MakeShared<DefaultEventManager>());
	else if (type == typeid(IMessageManager))
		return std::static_pointer_cast<void>(MakeShared<DefaultMessageManager>());
	else if (type == typeid(IMemoryManager))
		return std::static_pointer_cast<void>(MakeShared<DefaultMemoryManager>());
	else if (type == typeid(ITaskManager))
		return std::static_pointer_cast<void>(MakeShared<DefaultTaskManager>());
	else if (type == typeid(ICommand))
		return std::static_pointer_cast<void>(MakeShared<DefaultCommand>());

	return nullptr;
}

} // namespace RCLib::Impl