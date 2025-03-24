#include "DuoWidget.h"

#include <QBoxLayout>

namespace RCLib::Qt
{

DuoWidget::DuoWidget(QBoxLayout* pLayout)
  : QWidget()
  , m_pLayout(pLayout)
{
	// Take ownership of the layout
	setLayout(m_pLayout);
}

} // namespace RCLib::Qt