#pragma once
#include "RClib_Fwd.h"

#include <memory>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <unordered_map>

namespace RCLib
{
/**
 * @brief Factory interface for creating system components
 * @ingroup Core
 *
 * @details Provides a centralized system for creating all major components.
 * This factory system ensures proper initialization and dependency management.
 *
 * To implement custom components:
 * 1. Create your custom implementation class deriving from the appropriate interface
 * 2. Create your custom factory deriving from IFactory
 * 3. Override DoCreate for each type you want to support
 *
 * Example:
 * @code
 * class MyLogger : public ILogger { ... };
 *
 * class MyFactory : public IFactory {
 * protected:
 *     SharedPtr<void> DoCreate(const std::type_info& type, const CreateArgs& args) override {
 *         if (type == typeid(ILogger)) {
 *             return MakeShared<MyLogger>();
 *         }
 *         return nullptr;
 *     }
 * };
 * @endcode
 *
 * @see IManager
 * @see FactoryImpl
 */
class RCLIB_API IFactory
{
public:
	static IFactoryPtr Get();

	/**
	 * @brief Virtual destructor
	 */
	virtual ~IFactory() = default;

	/**
	 * @brief Creates an instance of the requested interface type
	 * @tparam T The interface type to create
	 * @return Smart pointer to the created instance
	 * @throw std::runtime_error if creation fails
	 */
	template<typename T>
	SharedPtr<T> Create()
	{
		SharedPtr<void> ptr = CreateImpl(typeid(T));
		if (!ptr)
		{
			throw std::runtime_error(std::string("Failed to create ") + typeid(T).name());
		}
		return std::static_pointer_cast<T>(ptr);
	}

protected:
	IFactory() = default;

	/**
	 * @brief Implementation method to be overridden by derived factories
	 * @param type The type information of the interface to create
	 * @param args The arguments to pass to the constructor
	 * @return void pointer to the created instance, must be castable to the requested type
	 */
	virtual SharedPtr<void> CreateImpl(const std::type_info& type) = 0;
};

} // namespace RCLib