#pragma once
#include "RCLib_Fwd.h"

namespace RCLib
{
/**
 * @brief Base exception class for RCLib
 */
class RCLIB_API IException
{
public:
	virtual ~IException()                     = default;
	virtual const char* what() const noexcept = 0;
};

} // namespace RCLib