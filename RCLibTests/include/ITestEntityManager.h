#pragma once
#include "ITest.h"
#include "RCLibTests_Fwd.h"

namespace RCLib::Tests
{
class RCLIB_TESTS_API ITestEntityManager : public ITest
{
protected:
	virtual void       CreateEntities()             = 0;
	virtual void       DestroyEntities()            = 0;
	virtual void       Stop()                       = 0;
	virtual IEntityPtr GetRandomEntity(bool remove) = 0;

public:
	virtual ~ITestEntityManager() = default;
};

} // namespace RCLib::Tests