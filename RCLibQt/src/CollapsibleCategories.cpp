#include "CollapsibleCategories.h"
#include "RCLibQt.h"

#include <QApplication>
#include <QPainter>
#include <QStyle>
#include <QStyledItemDelegate>

namespace RCLib::Qt
{

/**
 * @brief Custom item delegate for drawing expand/collapse arrows.
 * 
 * This delegate ensures that expand/collapse arrows are drawn for top-level items
 * (categories) even when there is no indentation. It also handles proper spacing
 * and visual appearance of the arrows.
 */
class CArrowDelegate : public QStyledItemDelegate
{
public:
	explicit CArrowDelegate(QTreeWidget* pTree)
		: QStyledItemDelegate(pTree)
		, m_pTree(pTree)
	{
	}

	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override
	{
		if (!index.parent().isValid()) // if it's a top level index (category)
		{
			// Draw the expand/collapse arrow
			const int spacing = 2;
			QStyleOptionViewItem optionCopy = option;
			optionCopy.rect.moveLeft(spacing);
			optionCopy.rect.setWidth(8);
			QApplication::style()->drawPrimitive(
				m_pTree->isExpanded(index) ? QStyle::PE_IndicatorArrowDown : QStyle::PE_IndicatorArrowRight,
				&optionCopy,
				painter
			);

			// Draw the item text without focus/selection highlighting
			optionCopy.state &= ~QStyle::State_HasFocus & ~QStyle::State_Selected;
			optionCopy.rect = option.rect.adjusted(optionCopy.rect.right() + spacing, 0, 0, 0);
			QStyledItemDelegate::paint(painter, optionCopy, index);
		}
		else
		{
			// For non-category items, use default painting
			QStyledItemDelegate::paint(painter, option, index);
		}
	}

private:
	QTreeWidget* m_pTree; ///< The tree widget we're drawing for
};

CollapsibleCategories::CollapsibleCategories(QWidget* parent)
	: QTreeWidget(parent)
{
	// Configure the tree widget appearance
	setHeaderHidden(true);
	setIndentation(0);
	setItemDelegate(new CArrowDelegate(this));

	// Configure scrolling behavior
	setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
	setVerticalScrollMode(ScrollMode::ScrollPerPixel);

	// Set up single-click expansion
	setExpandsOnDoubleClick(false);
	connect(this, &QTreeWidget::itemPressed, [this](const QTreeWidgetItem* pItem) {
		if (pItem->isExpanded())
			collapseItem(pItem);
		else
			expandItem(pItem);
	});
}

void CollapsibleCategories::AddCategory(const QString& category)
{
	// Only add the category if it doesn't already exist
	if (!m_categories.contains(category))
	{
		QTreeWidgetItem* pItem = new QTreeWidgetItem(this);
		pItem->setText(0, category);
		pItem->setExpanded(true);
		m_categories[category] = pItem;
	}
}

void CollapsibleCategories::AddItem(const QString& category, const QString& item)
{
	// Only add the item if the category exists
	if (m_categories.contains(category))
	{
		QTreeWidgetItem* pItem = new QTreeWidgetItem(m_categories[category]);
		pItem->setText(0, item);
	}
}

void CollapsibleCategories::Clear()
{
	m_categories.clear();
	QTreeWidget::clear();
}

bool CollapsibleCategories::eventFilter(QObject* object, QEvent* event)
{
	if (event->type() == QEvent::LayoutRequest)
	{
		// Handle layout updates for content widgets
		const int childrenCount = topLevelItemCount();
		for (int i = 0; i < childrenCount; ++i)
		{
			QTreeWidgetItem* pItem = topLevelItem(i)->child(0);
			QWidget* pItemWidget = itemWidget(pItem, 0);
			if (object == pItemWidget)
			{
				scheduleDelayedItemsLayout();
				break;
			}
		}
	}
	return QTreeWidget::eventFilter(object, event);
}

} // namespace RCLib::Qt
