#pragma once
#include "RCLibQt_Fwd.h"

#include <QModelIndex>
#include <vector>

namespace RCLib
{
namespace Qt
{

class RCLIB_QT_API GenericTreeItem
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

} // namespace Qt
} // namespace RCLib
