#pragma once
#include "Variable.h"
#include "SmartPtr.h"

class Object :public SmartPtrTarget
{
public:
	Object() {}
	virtual ~Object() = default;

};

using ObjectPtr = SmartPtr<Object>;