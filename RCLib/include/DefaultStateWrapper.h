#pragma once
#include "IStateWrapper.h"
#include "RCLib_Fwd.h"

#include <string>
#include <string_view>

namespace RCLib::Impl
{
/**
 * @brief Default implementation of the state wrapper
 * @tparam T The managed type
 */
template<typename T>
class DefaultStateWrapper : public IStateWrapper
{
public:
	/**
	 * @brief Constructor
	 * @param name The name of the manager
	 * @param managed The managed instance to wrap
	 * @param enableLogging Whether to enable logging (default: true)
	 */
	DefaultStateWrapper(std::string_view name, SharedPtr<T> managed, bool enableLogging = true)
	  : m_name(name)
	  , m_logEnabled(enableLogging)
	  , m_managed(std::move(managed))
	{
	}

	/**
	 * @brief Virtual destructor
	 */
	virtual ~DefaultStateWrapper() = default;

	// IStateWrapper interface implementation
	void Initialize() override
	{
		if (!m_managed)
			return;

		if (m_logEnabled)
			IEngine::Get().GetLogger()->Info(std::string("Initializing ").append(m_name));

		m_isInitialized = m_managed->OnInitialize();
	}

	void Update() override
	{
		if (!m_managed || !m_isInitialized)
			return;

		m_managed->OnUpdate();
	}

	void Release() override
	{
		if (!m_managed)
			return;

		if (m_logEnabled)
			IEngine::Get().GetLogger()->Info(std::string("Releasing ").append(m_name));

		m_managed->OnRelease();
		m_isInitialized = false;
	}

	bool IsInitialized() const override { return m_isInitialized; }

	std::string_view GetName() const override { return m_name; }
	void             SetLoggingEnabled(bool enable) override { m_logEnabled = enable; }
	SharedPtr<T>     Get() const { return m_managed; }

private:
	SharedPtr<T> m_managed;
	std::string  m_name;
	bool         m_isInitialized{false};
	bool         m_logEnabled{true};
};

} // namespace RCLib::Impl