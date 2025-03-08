#pragma once
#include <QtIncludes.h>

class CollapsibleCategories : public QTreeWidget
{
public:
	CollapsibleCategories(QWidget* pParent = nullptr);

	void AddCategory(const char* label, QWidget* pCategoryContentWidget);

	virtual bool eventFilter(QObject* object, QEvent* event) override;
};
