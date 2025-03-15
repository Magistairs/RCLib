#pragma once
#include <QtIncludes.h>

class SmallButton : public QPushButton
{
public:
	SmallButton(const QString& text, QBoxLayout* pLayout);
	virtual ~SmallButton() {}
};
