#pragma once
#include "IWidgetsFactory.h"
#include "RCLibQt_Fwd.h"

namespace RCLib::Qt::Impl
{

class RCLIB_QT_API DefaultWidgetsFactory : public RCLib::Qt::IWidgetsFactory
{
public:
	friend RCLib::Qt::IWidgetsFactory& RCLib::Qt::IWidgetsFactory::Get();

	DefaultWidgetsFactory()           = default;
	~DefaultWidgetsFactory() override = default;

	// IWidgetsFactory interface
	DuoWidget*             CreateDuoWidget(QBoxLayout* layout) override;
	FileSelectWidget*      CreateFileSelectWidget(QBoxLayout* layout, bool isDirectory = false) override;
	CollapsibleCategories* CreateCollapsibleCategories(QWidget* parent = nullptr) override;
	OutputLog*             CreateOutputLog(QWidget* parent = nullptr) override;
	TestRunnerWidget*      CreateTestRunnerWidget(QWidget* parent = nullptr) override;
	QPushButton*           CreateSmallButton(QWidget* parent = nullptr) override;
};

} // namespace RCLib::Qt::Impl