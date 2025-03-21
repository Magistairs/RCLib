#include "RCLibQt.h"
#include "GenericTreeModel.h"
#include "RCLib.h"

using namespace RCLib::Qt;

GenericTreeModel::GenericTreeModel(QObject* pParent)
  : QAbstractItemModel(pParent)
{
#ifdef ENABLE_TEST
	m_pModelTester = new QAbstractItemModelTester(this, QAbstractItemModelTester::FailureReportingMode::Fatal);
#endif
}

void GenericTreeModel::SetRoot(const SharedPtr<GenericTreeItem>& pRoot)
{
	beginResetModel();
	m_itemsToParentIndex.clear();
	m_pRoot = pRoot;
	endResetModel();
}

QModelIndex GenericTreeModel::index(int row, int column, const QModelIndex& parent) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	if (m_pRoot)
	{
		GenericTreeItem* pParentItem = nullptr;
		if (!parent.isValid())
			pParentItem = m_pRoot.get();
		else
			pParentItem = GetData(parent);

		GenericTreeItem* pChildItem      = pParentItem->GetChildren()[row];
		m_itemsToParentIndex[pChildItem] = parent;
		QModelIndex childIndex           = createIndex(row, column, pChildItem);
		pChildItem->SetIndex(childIndex);
		return childIndex;
	}
	return QModelIndex();
}

QModelIndex GenericTreeModel::parent(const QModelIndex& child) const
{
	if (m_pRoot && child.isValid())
	{
		GenericTreeItem* pItem  = GetData(child);
		QModelIndex      parent = m_itemsToParentIndex[pItem];

		return parent;
	}
	return QModelIndex();
}

int GenericTreeModel::rowCount(const QModelIndex& parent) const
{
	if (m_pRoot)
	{
		GenericTreeItem* pParentItem = nullptr;
		if (!parent.isValid())
			pParentItem = m_pRoot.get();
		else
			pParentItem = GetData(parent);

		return static_cast<int>(pParentItem->GetChildren().size());
	}
	return 0;
}

int GenericTreeModel::columnCount(const QModelIndex& parent) const
{
	return 1;
}

GenericTreeItem* GenericTreeModel::GetData(const QModelIndex& index)
{
	return static_cast<GenericTreeItem*>(index.internalPointer());
}
