#include "DuoWidget.h"
#include "RCLibQt.h"

#include <QBoxLayout>

using namespace RCLib::Qt;

DuoWidget::DuoWidget(QBoxLayout* pLayout)
  : QWidget()
  , m_pLayout(pLayout)
{
	setLayout(m_pLayout);
}