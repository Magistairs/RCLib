#pragma once
#include "RCLibQt_Fwd.h"

#include <QWidget>

class QBoxLayout;

namespace RCLib::Qt
{

/**
 * @brief Base class for widgets that combine two elements in a layout
 * 
 * Provides a foundation for creating widgets that combine two elements (like a label and input,
 * or two buttons) in a single layout. The layout type and orientation is configurable through
 * the constructor parameter.
 * 
 * This class is used as a base for more specialized widgets like FileSelectWidget.
 */
class RCLIB_QT_API DuoWidget : public QWidget
{
	Q_OBJECT

public:
	/**
	 * @brief Constructs a DuoWidget with the given layout
	 * @param pLayout The layout to use for arranging the elements
	 */
	explicit DuoWidget(QBoxLayout* pLayout);
	virtual ~DuoWidget() = default;

	/**
	 * @brief Gets the layout used by this widget
	 * @return The layout object
	 */
	QBoxLayout* GetLayout() const { return m_pLayout; }

protected:
	QBoxLayout* m_pLayout; ///< The layout used to arrange elements
};

} // namespace RCLib::Qt