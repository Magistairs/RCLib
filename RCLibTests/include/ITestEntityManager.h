#pragma once
#include "RCLibTests_Fwd.h"


namespace RCLib::Tests
{
class RCLIB_API ITestEntityManager
{
protected:
	virtual void       CreateEntities()              = 0;
	virtual void       DestroyEntities()             = 0;
	virtual void       Stop()                       = 0;
	virtual IEntityPtr GetRandomEntity(bool remove) = 0;

public:
	virtual ~ITestEntityManager() = default;
};

} // namespace RCLib::Tests