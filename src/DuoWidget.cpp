#include "DuoWidget.h"

using namespace RCLib;

DuoWidget::DuoWidget(QBoxLayout* pLayout)
	: QWidget(pLayout->parentWidget())
{
	QHBoxLayout* pInnerLayout = new QHBoxLayout(this);
	pLayout->addWidget(this);
}
