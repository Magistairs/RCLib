#include "GenericTreeView.h"
#include "RCLibQt.h"

namespace RCLib::Qt
{

GenericTreeView::GenericTreeView(QWidget* pParent)
  : QTreeView(pParent)
  , m_pModel(nullptr)
{
	// Configure the view for optimal display of tree data
	setUniformRowHeights(true);  // Optimize performance for items of same height
	setAlternatingRowColors(true);  // Improve readability
	setAnimated(true);  // Enable smooth animations for expand/collapse
}

void GenericTreeView::SetModel(GenericTreeModel* pModel)
{
	m_pModel = pModel;
	setModel(m_pModel);
	
	if (m_pModel)
	{
		// Expand the root item by default
		expand(m_pModel->index(0, 0, QModelIndex()));
	}
}

} // namespace RCLib::Qt
