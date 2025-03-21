#pragma once
#include "IFactory.h"
#include "RCLib_Fwd.h"

#include <typeinfo>

namespace RCLib::Impl
{
/**
 * @brief Default implementation of the factory system
 * @ingroup Core
 *
 * @details Creates default implementations for all system components.
 * Each component is created with its default configuration.
 *
 * Example of extending with custom implementations:
 * @code
 * class MyFactory : public DefaultFactory {
 * protected:
 *     template<typename... Args>
 *     SharedPtr<void> DoCreate(const std::type_info& type, Args&&... args) override {
 *         auto ptr = DefaultFactory::DoCreate(type, std::forward<Args>(args)...);
 *         if (!ptr) {
 *             if (type == typeid(IEvent)) {
 *                 return MakeShared<MyEventImpl>(std::forward<Args>(args)...);
 *             }
 *         }
 *         return ptr;
 *     }
 * };
 * @endcode
 *
 * @note All created instances are thread-safe and follow RAII principles
 *
 * @see IFactory
 * @see IManager
 */
class RCLIB_API DefaultFactory : public IFactory
{
public:
	friend IFactoryPtr IFactory::Get();

	/**
	 * @brief Default constructor
	 */
	DefaultFactory() = default;

	/**
	 * @brief Virtual destructor
	 */
	~DefaultFactory() override = default;

protected:
	/**
	 * @brief Creates default implementations for each interface
	 * @param type The type_info of the interface to create
	 * @param args The arguments to pass to the constructor
	 * @return SharedPtr<void> to the created instance, or nullptr if no implementation exists
	 */
	SharedPtr<void> CreateImpl(const std::type_info& type) override;
};

} // namespace RCLib::Impl