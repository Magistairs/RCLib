#include "GenericTreeItem.h"
#include "RCLibQt.h"

using namespace RCLib::Qt;

GenericTreeItem::GenericTreeItem() {}

GenericTreeItem::~GenericTreeItem()
{
	for (GenericTreeItem* pChild : m_children)
	{
		delete pChild;
	}
}

void GenericTreeItem::EmitDataChanged() const
{
	if (m_index.isValid())
	{
		Q_EMIT const_cast<QAbstractItemModel*>(m_index.model())->dataChanged(m_index, m_index);
	}
}
