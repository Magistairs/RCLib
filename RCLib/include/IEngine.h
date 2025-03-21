#pragma once
#include "IState.h"
#include "RCLib_Fwd.h"

namespace RCLib
{
/**
 * @brief Public interface for the engine system
 */
class RCLIB_API IEngine : public IState
{
public:
	static IEngine& Get();

	virtual ~IEngine() = default;

	// Manager registration
	virtual void RegisterCustomManager(IStateWrapperPtr manager) = 0;

	// Manager accessors
	virtual const ITaskManagerPtr    GetTaskManager()    = 0;
	virtual const IEntityManagerPtr  GetEntityManager()  = 0;
	virtual const ILoggerPtr         GetLogger()         = 0;
	virtual const ICommandManagerPtr GetCommandManager() = 0;
	virtual const IEventManagerPtr   GetEventManager()   = 0;
	virtual const IMessageManagerPtr GetMessageManager() = 0;
	virtual const IMemoryManagerPtr  GetMemoryManager()  = 0;

protected:
	IEngine() = default;
};

} // namespace RCLib