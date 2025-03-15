#include "CollapsibleCategories.h"

// Custom item delegate is needed because the default one doesn't render expand/collapse arrows if there is no indentation
class CArrowDelegate : public QStyledItemDelegate
{
public:
	CArrowDelegate(QTreeWidget* pTree)
		: QStyledItemDelegate(pTree)
		, m_pTree(pTree)
	{
	}

	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override
	{
		if (!index.parent().isValid()) // if it's a top level index
		{
			const int            spacing = 2;
			QStyleOptionViewItem optionCopy = option;
			optionCopy.rect.moveLeft(spacing);
			optionCopy.rect.setWidth(8);
			QApplication::style()->drawPrimitive(m_pTree->isExpanded(index) ? QStyle::PE_IndicatorArrowDown : QStyle::PE_IndicatorArrowRight, &optionCopy, painter);

			optionCopy.state &= ~QStyle::State_HasFocus & ~QStyle::State_Selected;
			optionCopy.rect = option.rect.adjusted(optionCopy.rect.right() + spacing, 0, 0, 0);
			QStyledItemDelegate::paint(painter, optionCopy, index);
		}
		else
		{
			QStyledItemDelegate::paint(painter, option, index);
		}
	}

private:
	QTreeWidget* m_pTree;
};

CollapsibleCategories::CollapsibleCategories(QWidget* pParent /*= nullptr*/)
	: QTreeWidget(pParent)
{
	setHeaderHidden(true);
	setIndentation(0);
	setItemDelegate(new CArrowDelegate(this));

	// No double scrollbar, items are always fully expanded
	setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
	setVerticalScrollMode(ScrollMode::ScrollPerPixel);

	// Expand on single click
	setExpandsOnDoubleClick(false);
	connect(this, &QTreeWidget::itemPressed, [this](const QTreeWidgetItem* pItem) {
		if (pItem->isExpanded())
			collapseItem(pItem);
		else
			expandItem(pItem);
		});
}

void CollapsibleCategories::AddCategory(const char* label, QWidget* pCategoryContentWidget)
{
	QTreeWidgetItem* pCategory = new QTreeWidgetItem(this, { label });
	pCategory->setExpanded(true);

	QTreeWidgetItem* pContainer = new QTreeWidgetItem(pCategory);
	pContainer->setDisabled(true); // disable highlight when selected
	setItemWidget(pContainer, 0, pCategoryContentWidget);

	pCategoryContentWidget->installEventFilter(this);
}

bool CollapsibleCategories::eventFilter(QObject* object, QEvent* event)
{
	if (event->type() == QEvent::LayoutRequest)
	{
		// We know that top level items are categories,
		// that categories have only 1 child which is the content widget
		// and that the content widgets events are listened here
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
