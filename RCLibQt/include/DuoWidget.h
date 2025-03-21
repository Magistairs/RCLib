#pragma once
#include "RCLibQt_Fwd.h"

#include <QWidget>

class QBoxLayout;

namespace RCLib
{
namespace Qt
{

/**
 * @brief Base class for widgets that combine two elements
 */
class RCLIB_QT_API DuoWidget : public QWidget
{
	Q_OBJECT

public:
	explicit DuoWidget(QBoxLayout* pLayout);
	virtual ~DuoWidget() = default;

	QBoxLayout* GetLayout() const { return m_pLayout; }

protected:
	QBoxLayout* m_pLayout;
};

} // namespace Qt
} // namespace RCLib