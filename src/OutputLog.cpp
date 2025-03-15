#include "OutputLog.h"

OutputLog::OutputLog(QWidget* pParent)
	: QListWidget(pParent)
{
}

void OutputLog::Log(QString str, ELogLevel level)
{
	addItem(str);
	QListWidgetItem* pItem = item(count() - 1);

	if (level == Warning)
		pItem->setForeground(QBrush(Qt::yellow));
	else if (level == Error)
		pItem->setForeground(QBrush(Qt::red));

	scrollToItem(pItem);
}
