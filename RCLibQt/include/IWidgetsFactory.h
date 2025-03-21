#pragma once
#include "RCLibQt_Fwd.h"

class QBoxLayout;
class QWidget;
class QPushButton;

namespace RCLib::Qt
{

// Forward declarations
class DuoWidget;
class FileSelectWidget;
class CollapsibleCategories;
class OutputLog;
class TestRunnerWidget;

// Factory interface for creating all widget types
class RCLIB_API IWidgetsFactory
{
public:
	static IWidgetsFactory& Get();

	virtual ~IWidgetsFactory() = default;

	// DuoWidget and derivatives
	virtual DuoWidget*        CreateDuoWidget(QBoxLayout* layout)                                  = 0;
	virtual FileSelectWidget* CreateFileSelectWidget(QBoxLayout* layout, bool isDirectory = false) = 0;

	// Tree and list widgets
	virtual CollapsibleCategories* CreateCollapsibleCategories(QWidget* parent = nullptr) = 0;
	virtual OutputLog*             CreateOutputLog(QWidget* parent = nullptr)             = 0;
	virtual TestRunnerWidget*      CreateTestRunnerWidget(QWidget* parent = nullptr)      = 0;

	// Buttons
	virtual QPushButton* CreateSmallButton(QWidget* parent = nullptr) = 0;

protected:
	IWidgetsFactory() = default;
};

} // namespace RCLib::Qt