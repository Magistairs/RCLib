#pragma once
#include "RCLibQt_Fwd.h"

#include <QModelIndex>
#include <vector>

namespace RCLib::Qt
{

/**
 * @brief A base class for items in a tree structure.
 * 
 * This class provides the basic functionality needed for items that can be displayed
 * in a tree view. Each item can have multiple children and maintains its position
 * in the model through a QModelIndex.
 */
class RCLIB_QT_API GenericTreeItem
{
public:
	/**
	 * @brief Constructs an empty GenericTreeItem.
	 */
	GenericTreeItem();

	/**
	 * @brief Destructor that cleans up all child items.
	 * 
	 * This destructor is responsible for deleting all child items in the tree,
	 * ensuring proper cleanup of the entire subtree.
	 */
	virtual ~GenericTreeItem();

	/**
	 * @brief Gets the list of child items.
	 * @return A reference to the vector containing pointers to child items.
	 */
	std::vector<GenericTreeItem*>& GetChildren() { return m_children; }

	/**
	 * @brief Sets the model index for this item.
	 * @param index The QModelIndex that represents this item's position in the model.
	 */
	void SetIndex(const QModelIndex& index) { m_index = index; }

	/**
	 * @brief Notifies the model that this item's data has changed.
	 * 
	 * This method emits the dataChanged signal through the model, causing
	 * any views displaying this item to update.
	 */
	void EmitDataChanged() const;

protected:
	QModelIndex m_index; ///< The model index that represents this item's position
	std::vector<GenericTreeItem*> m_children; ///< The list of child items owned by this item
};

} // namespace RCLib::Qt
