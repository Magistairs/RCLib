#pragma once
#include "RCLibQt_Fwd.h"

#include <map>
#include <memory>

#include <QAbstractItemModel>

namespace RCLib::Qt
{

/**
 * @brief A generic tree model that provides a hierarchical data structure for tree views.
 * 
 * This class implements the Qt model/view architecture for tree structures. It manages
 * a hierarchy of GenericTreeItem objects and provides the necessary interface for
 * QTreeView to display and interact with the data.
 */
class RCLIB_QT_API GenericTreeModel : public QAbstractItemModel
{
public:
	/**
	 * @brief Constructs a GenericTreeModel with the given parent.
	 * @param pParent The parent object for memory management.
	 */
	explicit GenericTreeModel(QObject* pParent);
	virtual ~GenericTreeModel() = default;

	/**
	 * @brief Sets the root item of the tree.
	 * @param pRoot The new root item to use.
	 * 
	 * This method resets the model and updates the root item. All views
	 * connected to this model will be notified of the change.
	 */
	void SetRoot(const SharedPtr<GenericTreeItem>& pRoot);

	// QAbstractItemModel overrides
	/**
	 * @brief Creates a model index for the given row and column.
	 */
	virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
	
	/**
	 * @brief Returns the parent of the model item with the given index.
	 */
	virtual QModelIndex parent(const QModelIndex& child) const override;
	
	/**
	 * @brief Returns the number of rows under the given parent.
	 */
	virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	
	/**
	 * @brief Returns the number of columns for the children of the given parent.
	 */
	virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;

	/**
	 * @brief Helper function to get the GenericTreeItem pointer from a model index.
	 */
	static GenericTreeItem* GetData(const QModelIndex& index);

protected:
	SharedPtr<GenericTreeItem> m_pRoot; ///< The root item of the tree
	mutable std::map<GenericTreeItem*, QModelIndex> m_itemsToParentIndex; ///< Cache of parent indices for quick lookup
};

} // namespace RCLib::Qt
