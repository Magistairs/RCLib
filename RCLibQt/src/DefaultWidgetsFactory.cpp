#include "RCLibQt.h"
#include "DefaultWidgetsFactory.h"
#include "DuoWidget.h"
#include "FileSelectWidget.h"
#include "CollapsibleCategories.h"
#include "OutputLog.h"

namespace RCLib::Widgets::Impl
{

DuoWidget* DefaultWidgetsFactory::CreateDuoWidget(QBoxLayout* layout)
{
    return new DuoWidget(layout);
}

FileSelectWidget* DefaultWidgetsFactory::CreateFileSelectWidget(QBoxLayout* layout, bool isDirectory)
{
    return new FileSelectWidget(layout, isDirectory ? FileSelectWidget::Directory : FileSelectWidget::File);
}

CollapsibleCategories* DefaultWidgetsFactory::CreateCollapsibleCategories(QWidget* parent)
{
    return new CollapsibleCategories(parent);
}

OutputLog* DefaultWidgetsFactory::CreateOutputLog(QWidget* parent)
{
    return new OutputLog(parent);
}

TestRunnerWidget* DefaultWidgetsFactory::CreateTestRunnerWidget(QWidget* parent)
{
    return new TestRunnerWidget(parent);
}

QPushButton* DefaultWidgetsFactory::CreateSmallButton(QWidget* parent)
{
    auto* pButton = new QPushButton(parent);
    pButton->setMaximumWidth(80);
    return pButton;
}

QPushButton* DefaultWidgetsFactory::CreateMediumButton()
{
    auto* pButton = new QPushButton();
    pButton->setMaximumWidth(120);
    return pButton;
}

QPushButton* DefaultWidgetsFactory::CreateLargeButton()
{
    auto* pButton = new QPushButton();
    pButton->setMaximumWidth(160);
    return pButton;
}

} // namespace RCLib::Widgets::Impl 