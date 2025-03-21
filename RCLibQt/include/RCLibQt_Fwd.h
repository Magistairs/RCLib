#pragma once

#include "RCLib_Fwd.h"

#ifdef _WIN32
#ifdef RCLIB_QT_EXPORT
#define RCLIB_QT_API __declspec(dllexport)
#else
#define RCLIB_QT_API __declspec(dllimport)
#endif
#else
#define RCLIB_QT_API
#endif

namespace RCLib
{
namespace Qt
{
class CollapsibleCategories;
class ConfigFile;
class ConfigValue;
class DuoWidget;
class FileSelectWidget;
class GenericTreeItem;
class GenericTreeModel;
class GenericTreeView;
class OutputLog;
class TestRunnerWidget;
class Variable;
} // namespace Qt
} // namespace RCLib