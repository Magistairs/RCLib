#include "GenericTreeModel.h"
#include "GenericTreeItem.h"
#include "RCLibQt.h"

namespace RCLib::Qt
{

GenericTreeModel::GenericTreeModel(QObject* pParent)
  : QAbstractItemModel(pParent)
  , m_pRoot(nullptr)
{
}

void GenericTreeModel::SetRoot(const SharedPtr<GenericTreeItem>& pRoot)
{
	// Notify views that we're about to make a major change
	beginResetModel();
	
	// Clear the parent index cache and update the root
	m_itemsToParentIndex.clear();
	m_pRoot = pRoot;
	
	// Notify views that the change is complete
	endResetModel();
}

QModelIndex GenericTreeModel::index(int row, int column, const QModelIndex& parent) const
{
	// Check if the requested index is valid
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	if (m_pRoot)
	{
		// Get the parent item - either root or the item from the parent index
		GenericTreeItem* pParentItem = nullptr;
		if (!parent.isValid())
			pParentItem = m_pRoot.get();
		else
			pParentItem = GetData(parent);

		// Create and cache the index for the child item
		GenericTreeItem* pChildItem = pParentItem->GetChildren()[row];
		m_itemsToParentIndex[pChildItem] = parent;
		QModelIndex childIndex = createIndex(row, column, pChildItem);
		pChildItem->SetIndex(childIndex);
		return childIndex;
	}
	return QModelIndex();
}

QModelIndex GenericTreeModel::parent(const QModelIndex& child) const
{
	// Return invalid index if we have no root or the child is invalid
	if (m_pRoot && child.isValid())
	{
		GenericTreeItem* pItem = GetData(child);
		// Look up the parent index from our cache
		QModelIndex parent = m_itemsToParentIndex[pItem];
		return parent;
	}
	return QModelIndex();
}

int GenericTreeModel::rowCount(const QModelIndex& parent) const
{
	if (m_pRoot)
	{
		// Get the parent item - either root or the item from the parent index
		GenericTreeItem* pParentItem = nullptr;
		if (!parent.isValid())
			pParentItem = m_pRoot.get();
		else
			pParentItem = GetData(parent);

		// Return the number of children
		return static_cast<int>(pParentItem->GetChildren().size());
	}
	return 0;
}

int GenericTreeModel::columnCount(const QModelIndex& parent) const
{
	// This is a single-column tree
	return 1;
}

GenericTreeItem* GenericTreeModel::GetData(const QModelIndex& index)
{
	// Extract the GenericTreeItem pointer stored in the index
	return static_cast<GenericTreeItem*>(index.internalPointer());
}
} // namespace RCLib::Qt