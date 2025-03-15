#pragma once
#include "GenericTreeItem.h"
#include <QtIncludes.h>

// #define ENABLE_TEST

namespace RCLib
{

class GenericTreeModel : public QAbstractItemModel
{
public:
	GenericTreeModel(QObject* pParent);
	virtual ~GenericTreeModel() {}

	void SetRoot(const std::shared_ptr<GenericTreeItem>& pRoot);

	// QAbstractItemModel overrides
	virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
	virtual QModelIndex parent(const QModelIndex& child) const override;
	virtual int         rowCount(const QModelIndex& parent = QModelIndex()) const override;
	virtual int         columnCount(const QModelIndex& parent = QModelIndex()) const override;

	static GenericTreeItem* GetData(const QModelIndex& index);

protected:
	std::shared_ptr<GenericTreeItem>                m_pRoot;
	mutable std::map<GenericTreeItem*, QModelIndex> m_itemsToParentIndex;

#ifdef ENABLE_TEST
	QAbstractItemModelTester* m_pModelTester{nullptr};
#endif
};
using GenericTreeModelPtr = std::shared_ptr<GenericTreeModel>;

} // namespace RCLib
