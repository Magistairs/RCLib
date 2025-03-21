#pragma once
#include "IEventManager.h"
#include "RCLib_Fwd.h"

#include <shared_mutex>
#include <unordered_map>
#include <vector>

namespace RCLib::Impl
{

class DefaultFactory;

/**
 * @brief Thread-safe implementation of the event manager
 * @ingroup Core
 */
class RCLIB_API DefaultEventManager : public IEventManager
{
public:
	explicit DefaultEventManager();
	~DefaultEventManager() override = default;

	// IEventManager interface implementation
	void AddEventListener(void* owner, TEventType eventID, FEventCallback callback) override;
	void RemoveEventListener(void* owner, TEventType eventID) override;
	void RemoveAllEventListeners(void* owner) override;
	void PostEvent(TEventType eventID) override;
	void ProcessEvents() override;

	// Managed interface implementation
	bool OnInitialize() override;
	bool OnUpdate() override;
	void OnRelease() override;

private:
	// Mutexes for thread safety
	mutable std::shared_mutex m_eventListenersMutex;
	mutable std::mutex        m_pendingEventsMutex;

	// Event storage
	std::unordered_map<TEventType, std::vector<std::pair<void*, FEventCallback>>> m_eventListeners;
	std::vector<TEventType>                                                       m_pendingEvents;
};

} // namespace RCLib::Impl