#include "OutputLog.h"
#include "RCLibQt.h"

namespace RCLib::Qt
{

OutputLog::OutputLog(QWidget* parent)
  : QListWidget(parent)
{
}

void OutputLog::AddMessage(const QString& message)
{
	addItem(message);
	scrollToBottom();
}

void OutputLog::Clear()
{
	QListWidget::clear();
}

void OutputLog::Log(QString str, ELogLevel level)
{
	addItem(str);
	QListWidgetItem* pItem = item(count() - 1);

	if (level == Warning)
		pItem->setForeground(QBrush(::Qt::yellow));
	else if (level == Error)
		pItem->setForeground(QBrush(::Qt::red));

	scrollToItem(pItem);
}
} // namespace RCLib::Qt