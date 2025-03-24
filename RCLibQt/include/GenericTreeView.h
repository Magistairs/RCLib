#pragma once
#include "GenericTreeModel.h"
#include "RCLibQt_Fwd.h"

#include <QTreeView>

namespace RCLib::Qt
{

/**
 * @brief A specialized tree view widget that works with GenericTreeModel.
 * 
 * This class provides a customized QTreeView implementation that is designed to work
 * seamlessly with GenericTreeModel. It handles the display and interaction with
 * hierarchical data structures represented by GenericTreeItems.
 */
class GenericTreeView : public QTreeView
{
	Q_OBJECT

public:
	/**
	 * @brief Constructs a GenericTreeView with the given parent widget.
	 * @param pParent The parent widget, or nullptr if the view has no parent.
	 */
	explicit GenericTreeView(QWidget* pParent = nullptr);
	virtual ~GenericTreeView() = default;

	/**
	 * @brief Sets the model for this view.
	 * @param pModel The GenericTreeModel to be used by this view.
	 * 
	 * This method sets up the model-view relationship and stores a pointer
	 * to the model for internal use.
	 */
	void SetModel(GenericTreeModel* pModel);

protected:
	GenericTreeModel* m_pModel; ///< The model being displayed by this view
};

} // namespace RCLib::Qt