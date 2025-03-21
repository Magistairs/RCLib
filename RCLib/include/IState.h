#pragma once
#include "DLLExport.h"

namespace RCLib
{
/**
 * @brief Base interface for objects with lifecycle management
 */
class RCLIB_API IState
{
public:
	virtual ~IState() = default;

	/**
	 * @brief Called during initialization phase
	 * @return true if initialization succeeded, false otherwise
	 */
	virtual bool OnInitialize() = 0;

	/**
	 * @brief Called during update phase
	 * @return true if update succeeded, false otherwise
	 */
	virtual bool OnUpdate() = 0;

	/**
	 * @brief Called during release phase
	 */
	virtual void OnRelease() = 0;

protected:
	IState() = default;
};

} // namespace RCLib