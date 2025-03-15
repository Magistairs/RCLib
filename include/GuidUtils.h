#pragma once
#include "DLLExport.h"

#include <guiddef.h>
#include <string>

namespace RCLib
{
	namespace GuidUtils
	{
		struct RCLIB_API GUIDComparer
		{
			bool operator()(REFGUID left, REFGUID right) const { return memcmp(&left, &right, sizeof(GUID)) < 0; }
		};
		RCLIB_API std::string GuidToString(const GUID& guid);
		RCLIB_API GUID        StringToGuid(const std::string& str);
	} // namespace GuidUtils
} // namespace RCLib
