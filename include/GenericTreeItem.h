#pragma once
#include <QtIncludes.h>

namespace RCLib {

class GenericTreeItem
{
public:
	GenericTreeItem();
	virtual ~GenericTreeItem();

	std::vector<GenericTreeItem*>& GetChildren() { return m_children; }
	void                           SetIndex(const QModelIndex& index) { m_index = index; }
	void                           EmitDataChanged() const;

protected:
	QModelIndex                   m_index;
	std::vector<GenericTreeItem*> m_children;
};

} // namespace RCLib
