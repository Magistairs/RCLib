#include "GenericTreeItem.h"

#include <QAbstractItemModel>

namespace RCLib::Qt
{

GenericTreeItem::GenericTreeItem()
	: m_index()  // Initialize with invalid index
{
}

GenericTreeItem::~GenericTreeItem()
{
	// Clean up all child items
	for (GenericTreeItem* pChild : m_children)
	{
		delete pChild;
	}
	m_children.clear();
}

void GenericTreeItem::EmitDataChanged() const
{
	// Only emit the signal if we have a valid index
	if (m_index.isValid())
	{
		// We need to const_cast here because the model() method returns a const pointer,
		// but we need to call a non-const method (dataChanged)
		Q_EMIT const_cast<QAbstractItemModel*>(m_index.model())->dataChanged(m_index, m_index);
	}
}

} // namespace RCLib::Qt
