#include "SmallButton.h"

using namespace RCLib;

SmallButton::SmallButton(const QString& text, QBoxLayout* pLayout)
	: QPushButton(text)
{
	pLayout->addWidget(this);
	setSizePolicy(QSizePolicy::Policy::Maximum, QSizePolicy::Policy::Maximum);
}
