#pragma once
#include "RCLib.h"
#include "QtLogger.h"
#include "DefaultTestRunner.h"

namespace RCLibQt
{

/**
 * @brief Factory for creating Qt-specific implementations of RCLib components
 */
class RCLIB_API QtFactory : public RCLib::Impl::DefaultFactory
{
public:
    QtFactory() = default;
    ~QtFactory() noexcept override = default;

protected:
    RCLib::SharedPtr<void> DoCreate(const std::type_info& type, const CreateArgs& args = {}) override
    {
        if (type == typeid(RCLib::ILogger))
        {
            return RCLib::MakeShared<QtLogger>();
        }
        else if (type == typeid(RCLib::ITestRunner))
        {
            return RCLib::MakeShared<RCLib::Impl::DefaultTestRunner>();
        }
        
        // For other types, use the default implementation
        return RCLib::Impl::DefaultFactory::CreateImpl(type, args);
    }

    // Add other Qt-specific implementations as needed
    // For example:
    // if (type == typeid(RCLib::ICommandManager))
    // {
    //     return RCLib::MakeShared<QtCommandManager>();
    // }
};

} // namespace RCLibQt 