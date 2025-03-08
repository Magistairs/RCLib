#pragma once
#include <QtIncludes.h>

class DuoWidget : public QWidget
{
public:
	DuoWidget(QBoxLayout* pLayout);
	virtual ~DuoWidget() {}

	QBoxLayout* GetLayout() const { return static_cast<QBoxLayout*>(layout()); }
};
