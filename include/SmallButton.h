#pragma once
#include "QtIncludes.h"

namespace RCLib {

class SmallButton : public QPushButton
{
public:
	SmallButton(const QString& text, QBoxLayout* pLayout);
	virtual ~SmallButton() {}
};

} // namespace RCLib
