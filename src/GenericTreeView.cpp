#include "GenericTreeView.h"
#include "GenericTreeModel.h"

using namespace RCLib;

GenericTreeView::GenericTreeView(QWidget* pParent)
  : QTreeView(pParent)
{
}

void GenericTreeView::SetModel(GenericTreeModelPtr pModel)
{
	m_pModel = pModel;
	setModel(m_pModel.get());
}
