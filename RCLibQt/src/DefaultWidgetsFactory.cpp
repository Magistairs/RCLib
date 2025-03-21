#include "DefaultWidgetsFactory.h"
#include "CollapsibleCategories.h"
#include "DuoWidget.h"
#include "FileSelectWidget.h"
#include "OutputLog.h"
#include "RCLibQt.h"

#include <QPushButton>

namespace RCLib::Qt::Impl
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
	return nullptr;
}

QPushButton* DefaultWidgetsFactory::CreateSmallButton(QWidget* parent)
{
	auto* pButton = new QPushButton(parent);
	pButton->setMaximumWidth(80);
	return pButton;
}

} // namespace RCLib::Qt::Impl