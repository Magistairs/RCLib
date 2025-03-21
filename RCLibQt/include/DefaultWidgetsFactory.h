#pragma once
#include "RCLibQt_Fwd.h"
#include "IWidgetsFactory.h"

namespace RCLib::Widgets::Impl
{

class RCLIBQT_API DefaultWidgetsFactory : public IWidgetsFactory
{
public:
    DefaultWidgetsFactory() = default;
    ~DefaultWidgetsFactory() override = default;

    // IWidgetsFactory interface
    DuoWidget* CreateDuoWidget(QBoxLayout* layout) override;
    FileSelectWidget* CreateFileSelectWidget(QBoxLayout* layout, bool isDirectory = false) override;
    CollapsibleCategories* CreateCollapsibleCategories(QWidget* parent = nullptr) override;
    OutputLog* CreateOutputLog(QWidget* parent = nullptr) override;
    TestRunnerWidget* CreateTestRunnerWidget(QWidget* parent = nullptr) override;
    QPushButton* CreateSmallButton(QWidget* parent = nullptr) override;
    QPushButton* CreateMediumButton() override;
    QPushButton* CreateLargeButton() override;
};

} // namespace RCLib::Widgets::Impl 