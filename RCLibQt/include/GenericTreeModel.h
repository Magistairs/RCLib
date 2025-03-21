#pragma once
#include "RCLibQt_Fwd.h"

#include <map>
#include <memory>

#include <QAbstractItemModel>

namespace RCLib
{
namespace Qt
{

class RCLIB_QT_API GenericTreeModel : public QAbstractItemModel
{
public:
	GenericTreeModel(QObject* pParent);
	virtual ~GenericTreeModel() {}

	void SetRoot(const SharedPtr<GenericTreeItem>& pRoot);

	// QAbstractItemModel overrides
	virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
	virtual QModelIndex parent(const QModelIndex& child) const override;
	virtual int         rowCount(const QModelIndex& parent = QModelIndex()) const override;
	virtual int         columnCount(const QModelIndex& parent = QModelIndex()) const override;

	static GenericTreeItem* GetData(const QModelIndex& index);

protected:
	SharedPtr<GenericTreeItem>                      m_pRoot;
	mutable std::map<GenericTreeItem*, QModelIndex> m_itemsToParentIndex;
};

} // namespace Qt
} // namespace RCLib
