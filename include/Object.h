#pragma once
#include "SmartPtr.h"

namespace RCLib {

class Object : public SmartPtrTarget
{
public:
	Object() {}
	virtual ~Object() = default;

};

using ObjectPtr = SmartPtr<Object>;

} // namespace RCLib