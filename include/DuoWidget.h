#pragma once
#include "QtIncludes.h"

namespace RCLib {

class DuoWidget : public QWidget
{
public:
	DuoWidget(QBoxLayout* pLayout);
	virtual ~DuoWidget() {}

	QBoxLayout* GetLayout() const { return static_cast<QBoxLayout*>(layout()); }
};

} // namespace RCLib
