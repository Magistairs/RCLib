#pragma once

// Essential includes
#include "DLLExport.h"
#include "MemoryTracking.h"
#include "SmartPointers.h"

#include <functional>

namespace RCLib
{

// Forward declarations for core managers
RCLIB_DECLARE_PTR(IFactory);
RCLIB_DECLARE_PTR(IState);
RCLIB_DECLARE_PTR(IStateWrapper);
RCLIB_DECLARE_PTR(IEngine);
RCLIB_DECLARE_PTR(IEntityManager);
RCLIB_DECLARE_PTR(IEventManager);
RCLIB_DECLARE_PTR(ILogger);
RCLIB_DECLARE_PTR(ITaskManager);
RCLIB_DECLARE_PTR(ICommandManager);
RCLIB_DECLARE_PTR(IMessageManager);
RCLIB_DECLARE_PTR(IMemoryManager);
RCLIB_DECLARE_PTR(ITestRunner);

// Forward declarations for core classes
RCLIB_DECLARE_PTR(IEntity);
RCLIB_DECLARE_PTR(ICommand);
RCLIB_DECLARE_PTR(IMessage);
RCLIB_DECLARE_PTR(IEvent);
RCLIB_DECLARE_PTR(ITest);

// Core types
using TEntityId    = uint32_t;
using TEventType   = size_t;
using TMessageType = size_t;

// Core function types
using FEventCallback  = std::function<void()>;
using FMessageHandler = std::function<void(const IMessage&)>;

// Core constants
constexpr TEntityId INVALID_ENTITY_ID = 0;

} // namespace RCLib