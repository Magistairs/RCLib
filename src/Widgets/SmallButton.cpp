#include "Widgets/SmallButton.h"

SmallButton::SmallButton(const QString& text, QBoxLayout* pLayout)
	: QPushButton(text)
{
	pLayout->addWidget(this);
	setSizePolicy(QSizePolicy::Policy::Maximum, QSizePolicy::Policy::Maximum);
}
